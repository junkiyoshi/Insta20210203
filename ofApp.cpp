#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetCircleResolution(72);

	this->number_of_targets = 360;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3(1, 1, 1));
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	int count = 0;
	int index = 0;
	int deg_start = ofGetFrameNum();
	for (int radius = 150; radius <= 300; radius += 15) {

		int deg_start = ofMap(ofNoise(radius * 0.005 + ofGetFrameNum() * 0.01), 0, 1, -180, 180);
		for (int deg = deg_start; deg < deg_start + 360; deg += 10) {

			this->target_list[index] = glm::vec2(360 + radius * cos(deg * DEG_TO_RAD), 360 + radius * sin(deg * DEG_TO_RAD));

			this->color_list[index] = glm::vec3(
				ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.05), 0, 1, 0.5, 1), 
				ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.05), 0, 1, 0.5, 1), 
				ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.05), 0, 1, 0.5, 1));

			index++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
bool ofApp::calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a) {

	auto distance = glm::distance(location_1, location_2);
	if (distance > radius_1 + radius_2 || distance < radius_1 - radius_2) { return false; }

	theta = atan2(location_2.y - location_1.y, location_2.x - location_1.x);
	auto cos_a = (pow(distance, 2) + pow(radius_1, 2) - pow(radius_2, 2)) / (2 * distance * radius_1);
	a = acos(cos_a);
	return true;
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}