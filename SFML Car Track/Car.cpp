#include "Car.h"

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	assert(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

Car::Car(int id, sf::Vector2f pos, InputManager *input, ConsoleManager *console, ResourceManager *resource, CheckPointManager* checkpoint, Track* trk)
	:ID(id), inputManager(input), consoleManager(console), resourceManager(resource), track(trk){

	//CheckPointTracker tracker(checkpoint);
	checkPointTracker = CheckPointTracker(checkpoint);
	nextCheckpointBounds = checkPointTracker.GetNextCheckpointBounds();

	//position setup
	position.x = pos.x / scale;
	position.y = pos.y / scale;	

	//track info
	trackShapes = track->GetTrackShapes();
	tileSize = track->GetTileWidth();
	infoFont = resourceManager->GetPixelFont();
	infoText.reserve(lineCount);
	for (int i = 0; i <= lineCount; ++i) {
		sf::Text text;
		text.setFont(*infoFont);
		text.scale(0.5f, 0.5f);
		text.setFillColor(sf::Color::Blue);		
		infoText.push_back(text);
	}	
	infoText[infoText.size() - 1].setString("ID: " + std::to_string(ID));

	//car sprite
	carBody.setTexture(*resourceManager->GetCarTexture());
	carBody.setScale(sf::Vector2f((cgToRear + cgToFront) / carBody.getTextureRect().width, (halfWidth * 2.0) / carBody.getTextureRect().height));		
	driftTexture = resourceManager->GetDriftTexture();

	//collision
	collisionBounds.setPosition(position);
	collisionBounds.setOutlineColor(sf::Color::Red);
	float padding = 5.f;
	collisionBounds.setPointCount(4);
	sf::Vector2f boundingBox = sf::Vector2f(carBody.getGlobalBounds().width * scale + padding, carBody.getGlobalBounds().height * scale + padding);
	collisionBounds.setPoint(0, sf::Vector2f(0,0));
	collisionBounds.setPoint(1, sf::Vector2f(0, boundingBox.y));
	collisionBounds.setPoint(2, sf::Vector2f(boundingBox.x, boundingBox.y));
	collisionBounds.setPoint(3, sf::Vector2f(boundingBox.x, 0));
	collisionBounds.setOrigin(boundingBox.x / 2, boundingBox.y / 2);
	collisionBounds.setFillColor(sf::Color::Transparent);
	collisionBounds.setOutlineThickness(1.f);
	collisionArea = boundingBox.x * boundingBox.y;

	//wheels
	for (int i = 0; i < 4; ++i) {
		sf::RectangleShape wheel;
		wheel.setSize(sf::Vector2f(wheelWidth * 4, wheelWidth * 2));
		wheel.setFillColor(sf::Color::Black);
		wheel.setOrigin(sf::Vector2f(wheelWidth * 2, wheelWidth));
		wheels.push_back(wheel);
	}		

	//distance lines checking area
	scanArea = sf::RectangleShape(sf::Vector2f(track->GetTileWidth() * 2, track->GetTileWidth() * 2));
	scanArea.setOrigin(sf::Vector2f(track->GetTileWidth(), track->GetTileWidth()));
	scanArea.setPosition(position);
	scanArea.setFillColor(sf::Color(255, 0, 0, 100));

	//global bounds
	globalBounds.setFillColor(sf::Color::Transparent);
	globalBounds.setOutlineColor(sf::Color::Blue);
	globalBounds.setOutlineThickness(1);
	
}

void Car::DoPhysics(float dt) {
	float sn = std::sin(heading);
	float cs = std::cos(heading);

	//get velocity in local car coords
	velocity_c.x = cs * velocity.x + sn * velocity.y;
	velocity_c.y = cs * velocity.y - sn * velocity.x;

	//Weight on axles based on centre of gravity and weight shift due to forward/reverse acceleration
	float axleWeightFront = mass * (axleWeightRatioFront * gravity - weightTransfer * accel_c.x * cgHeight / wheelBase);
	float axleWeightRear = mass * (axleWeightRatioRear * gravity + weightTransfer * accel_c.x * cgHeight / wheelBase);

	//resulting velocity of the weheels as result of the yaw rate of the car body
	//v = yawRate * r where r is the distance from axle to CG and yawRate (angular velocity) in rad/s
	float yawSpeedFront = cgToFrontAxle * yawRate;
	float yawSpeedRear = -cgToRearAxle * yawRate;

	//calculate slip angles for front and rear wheels
	float slipAngleFront = std::atan2(velocity_c.y + yawSpeedFront, std::abs(velocity_c.x)) - sgn(velocity_c.x) * steerAngle;
	float slipAngleRear = std::atan2(velocity_c.y + yawSpeedRear, std::abs(velocity_c.x));

	if (slipAngleRear > 0.5 || slipAngleRear < -0.5) {
		addSkidMarks();
	}

	float tireGripFront = tireGrip;
	float tireGripRear = tireGrip * (1.0 - inputManager->GetEBrake() * (1.0 - lockGrip)); // reduce rear grip when ebrake is on

	float frictionForceFront_cy = clamp(-cornerStiffnessFront * slipAngleFront, -tireGripFront, tireGripFront) * axleWeightFront;
	float frictionForceRear_cy = clamp(-cornerStiffnessRear * slipAngleRear, -tireGripRear, tireGripRear) * axleWeightRear;

	//get amount of brake/throttle from inputs
	float brake(0), throttle(0);
	if (selected) {
		brake = std::min(inputManager->GetBrake() * brakeForce + inputManager->GetEBrake() * eBrakeForce, brakeForce);
		throttle = inputManager->GetThrottle() * engineForce;
	}

	//resulting force in local car coordinates
	float tractionForce_cx = throttle - brake * sgn(velocity_c.x);
	float tractionForce_cy = 0;

	float dragForce_cx = -rollResist * velocity_c.x - airResist * velocity_c.x * std::abs(velocity_c.x);
	float dragForce_cy = -rollResist * velocity_c.y - airResist * velocity_c.y * std::abs(velocity_c.y);

	//total force in car coords
	float totalForce_cx = dragForce_cx + tractionForce_cx;
	float totalForce_cy = dragForce_cy + tractionForce_cy + std::cos(steerAngle) * frictionForceFront_cy + frictionForceRear_cy;

	//acceleration along car axes
	accel_c.x = totalForce_cx / mass;
	accel_c.y = totalForce_cy / mass;

	//acceleration in world coords
	accel.x = cs * accel_c.x - sn * accel_c.y;
	accel.y = sn * accel_c.x + cs * accel_c.y;

	//update velocity
	velocity.x += accel.x * dt;
	velocity.y += accel.y * dt;

	absVel = std::hypot(velocity.x, velocity.y); //length of vector?

	//calculate rotational forces 
	float angularTorque = (frictionForceFront_cy + tractionForce_cy) * cgToFrontAxle - frictionForceRear_cy * cgToRearAxle;

	//unstable at slow speeds just stop car
	if (std::abs(absVel) < 0.5 && !throttle) {
		velocity.x = velocity.y = absVel = 0;
		angularTorque = yawRate = 0;
	}

	float angularAccel = angularTorque / inertia;

	yawRate += angularAccel * dt;
	heading += yawRate * dt;

	//update position
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

float Car::ApplySmoothSteer(float steerInput, float dt) {
	float steer_l = 0;

	if (std::abs(steerInput) > 0.001)
		steer_l = clamp((steer + (double)steerInput * (double)dt * 2.0), -1.0, 1.0);
	else {
		if (steer > 0)
			steer_l = std::max((steer - dt * 1.0), 0.0);
		else if (steer < 0)
			steer_l = std::min(steer + dt * 1.0, 0.0);
	}
	
	return steer_l;
}

float Car::ApplySafeSteer(float steerInput) {	
	float avel = std::min((double)absVel, 90.0);
	float steer_l = steerInput * (1.0 - (avel / 4000.0));
	return steer_l;
}

void Car::Update(float dt) {

	//inputs	
	float steerInput = 0;
	if (selected)
		steerInput = inputManager->GetSteerRight() - inputManager->GetSteerLeft();

	if (smoothSteer)
		steer = ApplySmoothSteer(steerInput, dt);
	else steer = steerInput;

	if (safeSteer)
		steer = ApplySafeSteer(steer);

	steerAngle = steer * maxSteer;

	if (velocity != sf::Vector2f(0, 0) || inputManager->GetThrottle() != 0) {
		DoPhysics(dt);
		scanArea.setPosition(position.x * scale, position.y * scale);
		if (selected)
			CalculateDistances();
	}

	infoText[infoText.size() - 1].setPosition(collisionBounds.getTransform().transformPoint(collisionBounds.getPoint(3)));

	CheckPointHandling();

	if (selected) {
		consoleManager->UpdateMessageValue("steer angle", std::to_string(steerAngle));
		consoleManager->UpdateMessageValue("velocity.x", std::to_string(velocity.x));
		consoleManager->UpdateMessageValue("velocity.y", std::to_string(velocity.y));
		consoleManager->UpdateMessageValue("skid count", std::to_string(skidMarks.size()));
		consoleManager->UpdateMessageValue("current segment", std::to_string(checkPointTracker.getCurrentSegmentTime() / 1000.f));
		consoleManager->UpdateMessageValue("fastest time", std::to_string(checkPointTracker.getFastestTime() / 1000.f));
		consoleManager->UpdateMessageValue("last lap", std::to_string(checkPointTracker.getLastLapTime() / 1000.f));
	}
}

void Car::Draw(sf::RenderWindow& window){
	
	//dev mode below car
	if (selected) {
		if (consoleManager->IsDisplayed()) {
			//distance lines
			for (auto& l : distanceLines)
				window.draw(l);
		}
	}

	//skid marks
	sf::Transform skidTransform;
	skidTransform.scale(scale, scale);
	for (sf::Sprite& c : skidMarks) 
		window.draw(c, skidTransform);

	sf::Transform transform;	
	transform.translate((int)(position.x * scale), (int)(position.y * scale));	
	transform.scale(scale, scale);
	transform.rotate(heading * (180 / M_PI));
	//front wheels
	wheels.at(0).setPosition(cgToFrontAxle, halfWidth);
	wheels.at(1).setPosition(cgToFrontAxle, -halfWidth);
	wheels.at(0).setRotation((steerAngle * (180 / M_PI)));
	wheels.at(1).setRotation((steerAngle * (180 / M_PI)));

	//back wheels
	wheels.at(2).setPosition(-cgToRearAxle, halfWidth);
	wheels.at(3).setPosition(-cgToRearAxle, -halfWidth);
	wheels.at(2).setRotation(0);
	wheels.at(3).setRotation(0);

	//draw wheels
	window.draw(wheels.at(0), transform); 
	window.draw(wheels.at(1), transform);
	window.draw(wheels.at(2), transform);
	window.draw(wheels.at(3), transform);

	//car body
	sf::Transform transform2;	
	transform2.translate(position.x * scale, position.y * scale);
	transform2.scale(scale, scale);

	carBody.setPosition(0,0);			
	carBody.setOrigin(carBody.getTextureRect().width / 2.0, carBody.getTextureRect().height / 2.0);
	carBody.setRotation(heading * (180 / M_PI));		

	collisionBounds.setPosition((int)(position.x * scale), (int)(position.y * scale));
	collisionBounds.setRotation(heading * (180 / M_PI));	

	sf::FloatRect glbl = collisionBounds.getGlobalBounds();
	globalBounds.setPosition(glbl.left+5, glbl.top+5);
	globalBounds.setSize(sf::Vector2f(glbl.width-10, glbl.height-10));

	window.draw(carBody, transform2);		

	//devmode (above car)
	if (consoleManager->IsDisplayed()) {
		//line distances
		if (selected) {
			for (auto& t : infoText) {
				window.draw(t);
			}
			window.draw(globalBounds);
		}
		window.draw(collisionBounds);
	}
}

void Car::addSkidMarks() {
	//temp skid func
	sf::Sprite temp1;	
	float cs = cos(heading);
	float sn = sin(heading);
	float posX = (position.x) - (cs * cgToRearAxle) - (sn * halfWidth);
	float posY = (position.y) - (sn * cgToRearAxle) + (cs * halfWidth);

	//right
	temp1.setScale(carBody.getScale());
	temp1.setTexture(*driftTexture);
	temp1.setOrigin(sf::Vector2f(temp1.getTextureRect().width / 2, temp1.getTextureRect().height / 2));
	temp1.setPosition(posX, posY);
	temp1.setRotation(heading * (180 / M_PI));
	skidMarks.push_back(temp1);	

	//left	
    posX = (position.x) - (cs * cgToRearAxle) + (sn * halfWidth);
	posY = (position.y) - (sn * cgToRearAxle) - (cs * halfWidth);
	temp1.setPosition(posX, posY);
	skidMarks.push_back(temp1);	
}

void Car::CalculateDistances() {
	distanceLines.clear();
	for (int i = 0; i < lineCount; ++i) {
		sf::VertexArray newLine = sf::VertexArray(sf::LinesStrip);
		newLine.append(sf::Vertex(sf::Vector2f(position.x * scale, position.y * scale), lineColor));

		const float theta = (M_PI * 2) / lineCount;
		const float angle = (theta * i);

		lin::Line line1 = lin::Line(sf::Vector2f(position.x * scale, position.y * scale), sf::Vector2f((position.x * scale) + (lineLength * cos(angle + heading)),
												 (position.y * scale) + (lineLength * sin(angle + heading))));

		if(consoleManager->IsDisplayed())track->clearCheckedArea();
		float shortestDistance = lineLength;
		for (auto& trackShape : *trackShapes) {	
			if (trackShape.getGlobalBounds().intersects(scanArea.getGlobalBounds())) {								
				if(consoleManager->IsDisplayed()) track->addCheckedArea(trackShape);				

				for (int i = 0; i < trackShape.getPointCount(); ++i) {
					lin::Line line2;
					if (i == trackShape.getPointCount() - 1) line2 = lin::Line(trackShape.getPoint(i), trackShape.getPoint(0));
					else line2 = lin::Line(trackShape.getPoint(i), trackShape.getPoint(i + 1));

					float testLen = std::sqrt(std::pow(line2.p1.x - line2.p2.x, 2) + std::pow(line2.p1.y - line2.p2.y, 2));
					float trackWidth = (tileSize / 8) * 4;
					if (testLen < tileSize * 0.2 || testLen > tileSize * 0.9) {
						float det = (line1.A * line2.B) - (line2.A * line1.B);
						if (det != 0) { //if not parallel
							float x = (line2.B * line1.C - line1.B * line2.C) / det;
							float y = (line1.A * line2.C - line2.A * line1.C) / det;
							if ((std::min(line2.p1.x, line2.p2.x) <= x + 1) && (x - 1 <= std::max(line2.p1.x, line2.p2.x)) &&
								(std::min(line2.p1.y, line2.p2.y) <= y + 1) && (y - 1 <= std::max(line2.p1.y, line2.p2.y)) &&
								(std::min(line1.p1.x, line1.p2.x) <= x + 1) && (x - 1 <= std::max(line1.p1.x, line1.p2.x)) &&
								(std::min(line1.p1.y, line1.p2.y) <= y + 1) && (y - 1 <= std::max(line1.p1.y, line1.p2.y))) {
								float length = std::sqrt(std::pow(line1.p1.x - x, 2) + std::pow(line1.p1.y - y, 2));
								if (shortestDistance > length) {
									line1.p2 = sf::Vector2f(x, y);
									shortestDistance = length;
								}
							}
						}
					}
				}
			}
		}
		int text_x = line1.p2.x;
		int text_y = line1.p2.y;
		infoText[i].setPosition(sf::Vector2f(text_x, text_y));
		std::string dist = std::to_string(shortestDistance);
		infoText[i].setString(dist.substr(0, dist.find_first_of('.') + 2));
		newLine.append(sf::Vertex(line1.p2, lineColor));
		distanceLines.push_back(newLine);
	}	
}

bool Car::containsPoint(sf::Vector2f P) {
	sf::Vector2f A = collisionBounds.getTransform().transformPoint(collisionBounds.getPoint(0));
	sf::Vector2f B = collisionBounds.getTransform().transformPoint(collisionBounds.getPoint(1));
	sf::Vector2f C = collisionBounds.getTransform().transformPoint(collisionBounds.getPoint(2));
	sf::Vector2f D = collisionBounds.getTransform().transformPoint(collisionBounds.getPoint(3));

	return lin::doesRectContainPoint(P, A, B, C, D);
}

void Car::CheckPointHandling() {
	if (globalBounds.getGlobalBounds().intersects(nextCheckpointBounds)) {
		checkPointTracker.CompleteSegment();
		nextCheckpointBounds = checkPointTracker.GetNextCheckpointBounds();
	}
}