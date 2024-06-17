#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

	// �����K�i�̍쐬
	double pi = 3.141592653589;
	//testmesh();
	meshSpiralStaircase(200, 100, 3, pi / 8);
	getOnering(mesh);
}

//--------------------------------------------------------------
void ofApp::testmesh() {
	ofMesh mesh_tmp;

	// �O�p���̍쐬
	mesh_tmp.addVertex(ofVec3f(0, 0, 0));
	mesh_tmp.addVertex(ofVec3f(100, 0, 0));
	mesh_tmp.addVertex(ofVec3f(0, 200, 0));
	mesh_tmp.addVertex(ofVec3f(0, 0, 300));

	// ���v��肪���A�����v��肪�\
	mesh_tmp.addIndices({ 0, 2, 1 });
	mesh_tmp.addIndices({ 0, 1, 3 });
	mesh_tmp.addIndices({ 0, 3, 2 });
	mesh_tmp.addIndices({ 1, 2, 3 });

	mesh = mesh_tmp;
}

void ofApp::getOnering(ofMesh mesh) {
	// �f�o�b�O�p�̔z����o�͂���֐�
	auto print_v = [](string s, vector<vector<int>>& v) {
		cout << s << endl;
		for (auto i : v) {
			for (auto j : i) {
				cout << j << " ";
			}
			cout << endl;
		}
		cout << endl;
	};

	// (1) triList�̍쐬----------------------------------
	vector<vector<int>> triList;

	// �ڑ����Ă��钸�_����擾
	vector<unsigned int> triInd = mesh.getIndices();
	// for (auto i : triInd) cout << i << " "; cout << "\n";  // triInd���o�͂��Ċm�F

	// triInd���璸�_3���Ƃ�triList��push_back
	for (int i = 0; i < triInd.size();){
		vector<int> tmp;
		for (int j = 0; j < 3; j++) {
			tmp.push_back(triInd[i]);
			i++;
		}
		triList.push_back(tmp);
	}

	print_v("triList", triList);  // triList���o��


	// (2) cnctface�̍쐬----------------------------------
	// �z��̐錾
	int pnum = mesh.getNumVertices();  // pnum = ���_��
	vector<vector<int>> cnctface(pnum);

	int triIdx = 0;
	for (int i = 0; i < triInd.size();) {
		for (int j = 0; j < 3; j++) {
			cnctface[triInd[i]].push_back(triIdx);
			i++;
		}
		triIdx++;
	}
	
	print_v("cnctface", cnctface);  // cnctface���o�� 


	// (3) onering�̍쐬--------------------------------
	vector<vector<int>> onering(pnum);

	for (int i = 0; i < pnum; i++) {
		map<int, int> m;
		int st = -1;  // �n�_

		// ���ׂĂ̗אڎO�p�`�ɂ����āAmap[p] = nexp�̗L���O���t���\�z
		// j�͗אڎO�p�`,k�͎O�p�`�̒��_�̃��[�v
		for(auto j : cnctface[i]){
			for (int k = 0; k < 3; k++) {
				if (triList[j][k] == i) {
					if (st == -1) {
						st = triList[j][(k + 1) % 3];
					}
					m[triList[j][(k + 1) % 3]] = triList[j][(k + 2) % 3];
				}
			}
		}

		onering[i].push_back(st);
		int idx = st;
		// �n�_�ɖ߂��Ă���܂ŗL���O���t�����ǂ�Aonering�ɒǉ�
		while(m[idx] != st) {
			onering[i].push_back(m[idx]);
			idx = m[idx];
		}

	}

	print_v("onering", onering);
	
}

//--------------------------------------------------------------
void ofApp::rotate(double& x, double& y, double theta) {
	double tmp_x = x * cos(theta) - y * sin(theta);
	double tmp_y = x * sin(theta) + y * cos(theta);
	x = tmp_x;
	y = tmp_y;
}

//--------------------------------------------------------------
void ofApp::meshSpiralStaircase(double r, double h, unsigned n, double theta) {
	ofMesh mesh_tmp;

	// ���_�̐���

	// ���S�̒��_�̐���
	// 1�ڂ̒��_��0, �Ō�̒��_��n+2
	for (int i = 0; i < n + 3; i++) {
		mesh_tmp.addVertex(ofVec3f(0, 0, h / 3 * i));
	}

	// �O���̒��_�̐���
	// 1�ڂ̒��_��n+3 �Ō�̒��_��n+3 + (n-1)*4+6
	double posx = r, posy = 0, posz = 0;

	// ��ԉ�
	for (int i = 0; i < 4; i++) {
		mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * i)));
	}
	rotate(posx, posy, theta);

	// �^��
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < 5; j++) {

			mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * j)));
		}
		rotate(posx, posy, theta);
		posz += h / 3;
	}

	// ��ԏ�
	for (int i = 0; i < 4; i++) {
		mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * i)));
	}

	// ��ƂȂ钸�_p
	unsigned p;

	// ���_�̐ڑ�

	// ���
	for (unsigned i = 0; i < n; i++) {
		p = n + 3 + i * 5;
		mesh_tmp.addIndices({ i, p + 4, p });
	}

	// ���
	for (unsigned i = 0; i < n; i++) {
		p = n + 6 + i * 5;
		mesh_tmp.addIndices({ i + 3, p, p + 4 });
	}

	// �O�ʂƔw��
	for (unsigned i = 0; i < n; i++) {
		p = n + 5 + i * 5;
		mesh_tmp.addIndices({ i + 2, p, i + 3 });
		mesh_tmp.addIndices({ p, p + 1, i + 3 });

		p += 2;
		mesh_tmp.addIndices({ p, i, p + 1 });
		mesh_tmp.addIndices({ i, i + 1, p + 1 });
	}


	// ��ԉ��̎O�p���̑O��
	mesh_tmp.addIndices({ 0, n + 3,  1 });
	mesh_tmp.addIndices({ 1, n + 4, 2 });
	mesh_tmp.addIndices({ n + 3, n + 4, 1 });
	mesh_tmp.addIndices({ n + 4, n + 5, 2 });

	// ��ԏ�̎O�p���̔w��
	p = 6 * n + 3;
	mesh_tmp.addIndices({ p, n, p + 1 });
	mesh_tmp.addIndices({ p + 1, n + 1, p + 2 });
	mesh_tmp.addIndices({ n, n + 1, p + 1 });
	mesh_tmp.addIndices({ n + 1, n + 2, p + 2 });

	// ����
	for (unsigned i = 0; i < n; i++) {
		p = n + 3 + i * 5;
		for (unsigned j = 0; j < 3; j++) {
			mesh_tmp.addIndices({ p + j, p + 4 + j, p + 1 + j });
			mesh_tmp.addIndices({ p + 4 + j, p + 5 + j , p + 1 + j });
		}
	}

	mesh = mesh_tmp;
}

//--------------------------------------------------------------
void ofApp::displayPointIndex(ofMesh mesh) {

	ofBackgroundGradient(ofColor(64), ofColor(0));

	ofSetColor(255);
	cam.begin();

	ofSetColor(ofColor::gray);
	mesh.drawWireframe();

	glPointSize(2);
	ofSetColor(ofColor::white);
	mesh.drawVertices();
	cam.end();
	
	int n = mesh.getNumVertices();
	glm::vec2 offset(10, -10);
	for (int i = 0; i < n; i++) {
		glm::vec3 cur = cam.worldToScreen(mesh.getVertex(i));
		ofSetColor(ofColor::yellow);
		ofDrawBitmapStringHighlight(ofToString(i), cur.x + offset.x, cur.y + offset.y);
	}

	/*
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	glm::vec2 nearestVertex;
	int nearestIndex = 0;
	glm::vec3 mouse(mouseX, mouseY, 0);
	for (int i = 0; i < n; i++) {
		glm::vec3 cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = glm::distance(cur, mouse);
		if (i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}

	ofSetColor(ofColor::gray);
	ofDrawLine(nearestVertex, mouse);

	ofNoFill();
	ofSetColor(ofColor::yellow);
	ofSetLineWidth(2);
	ofDrawCircle(nearestVertex, 4);
	ofSetLineWidth(1);

	glm::vec2 offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
	*/
}

//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {
	/*
	ofBackgroundGradient(ofColor(64), ofColor(0));

	ofSetColor(255);
	glEnable(GL_CULL_FACE);

	cam.begin();
	// X��, Y���CZ����`��
	glLineWidth(1.0);
	ofSetColor(ofColor::red);
	ofDrawLine(0, 0, 0, ofGetHeight() / 2, 0, 0);
	ofSetColor(ofColor::green);
	ofDrawLine(0, 0, 0, 0, ofGetHeight() / 2, 0);
	ofSetColor(ofColor::blue);
	ofDrawLine(0, 0, 0, 0, 0, ofGetHeight() / 2);

	ofSetColor(ofColor::gray);				// �|���S��(��)�̐F�̐ݒ�
	mesh.drawFaces();

	ofSetColor(ofColor::blue);			// �|���S��(�Ő�)�̐F�̐�
	glLineWidth(2.0);
	mesh.drawWireframe();

	ofSetColor(ofColor::white);			// �|���S��(���_)�̐F�̐ݒ�
	glPointSize(3.0);
	mesh.drawVertices();

	cam.end();
	*/

	displayPointIndex(mesh);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
