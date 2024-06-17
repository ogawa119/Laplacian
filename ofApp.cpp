#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

	// 螺旋階段の作成
	double pi = 3.141592653589;
	//testmesh();
	meshSpiralStaircase(200, 100, 3, pi / 8);
	getOnering(mesh);
}

//--------------------------------------------------------------
void ofApp::testmesh() {
	ofMesh mesh_tmp;

	// 三角錐の作成
	mesh_tmp.addVertex(ofVec3f(0, 0, 0));
	mesh_tmp.addVertex(ofVec3f(100, 0, 0));
	mesh_tmp.addVertex(ofVec3f(0, 200, 0));
	mesh_tmp.addVertex(ofVec3f(0, 0, 300));

	// 時計回りが裏、反時計回りが表
	mesh_tmp.addIndices({ 0, 2, 1 });
	mesh_tmp.addIndices({ 0, 1, 3 });
	mesh_tmp.addIndices({ 0, 3, 2 });
	mesh_tmp.addIndices({ 1, 2, 3 });

	mesh = mesh_tmp;
}

void ofApp::getOnering(ofMesh mesh) {
	// デバッグ用の配列を出力する関数
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

	// (1) triListの作成----------------------------------
	vector<vector<int>> triList;

	// 接続している頂点列を取得
	vector<unsigned int> triInd = mesh.getIndices();
	// for (auto i : triInd) cout << i << " "; cout << "\n";  // triIndを出力して確認

	// triIndから頂点3つごとにtriListにpush_back
	for (int i = 0; i < triInd.size();){
		vector<int> tmp;
		for (int j = 0; j < 3; j++) {
			tmp.push_back(triInd[i]);
			i++;
		}
		triList.push_back(tmp);
	}

	print_v("triList", triList);  // triListを出力


	// (2) cnctfaceの作成----------------------------------
	// 配列の宣言
	int pnum = mesh.getNumVertices();  // pnum = 頂点数
	vector<vector<int>> cnctface(pnum);

	int triIdx = 0;
	for (int i = 0; i < triInd.size();) {
		for (int j = 0; j < 3; j++) {
			cnctface[triInd[i]].push_back(triIdx);
			i++;
		}
		triIdx++;
	}
	
	print_v("cnctface", cnctface);  // cnctfaceを出力 


	// (3) oneringの作成--------------------------------
	vector<vector<int>> onering(pnum);

	for (int i = 0; i < pnum; i++) {
		map<int, int> m;
		int st = -1;  // 始点

		// すべての隣接三角形において、map[p] = nexpの有向グラフを構築
		// jは隣接三角形,kは三角形の頂点のループ
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
		// 始点に戻ってくるまで有向グラフをたどり、oneringに追加
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

	// 頂点の生成

	// 中心の頂点の生成
	// 1つ目の頂点は0, 最後の頂点はn+2
	for (int i = 0; i < n + 3; i++) {
		mesh_tmp.addVertex(ofVec3f(0, 0, h / 3 * i));
	}

	// 外側の頂点の生成
	// 1つ目の頂点はn+3 最後の頂点はn+3 + (n-1)*4+6
	double posx = r, posy = 0, posz = 0;

	// 一番下
	for (int i = 0; i < 4; i++) {
		mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * i)));
	}
	rotate(posx, posy, theta);

	// 真ん中
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < 5; j++) {

			mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * j)));
		}
		rotate(posx, posy, theta);
		posz += h / 3;
	}

	// 一番上
	for (int i = 0; i < 4; i++) {
		mesh_tmp.addVertex(ofVec3f(posx, posy, posz + (h / 3 * i)));
	}

	// 基準となる頂点p
	unsigned p;

	// 頂点の接続

	// 底面
	for (unsigned i = 0; i < n; i++) {
		p = n + 3 + i * 5;
		mesh_tmp.addIndices({ i, p + 4, p });
	}

	// 上面
	for (unsigned i = 0; i < n; i++) {
		p = n + 6 + i * 5;
		mesh_tmp.addIndices({ i + 3, p, p + 4 });
	}

	// 前面と背面
	for (unsigned i = 0; i < n; i++) {
		p = n + 5 + i * 5;
		mesh_tmp.addIndices({ i + 2, p, i + 3 });
		mesh_tmp.addIndices({ p, p + 1, i + 3 });

		p += 2;
		mesh_tmp.addIndices({ p, i, p + 1 });
		mesh_tmp.addIndices({ i, i + 1, p + 1 });
	}


	// 一番下の三角柱の前面
	mesh_tmp.addIndices({ 0, n + 3,  1 });
	mesh_tmp.addIndices({ 1, n + 4, 2 });
	mesh_tmp.addIndices({ n + 3, n + 4, 1 });
	mesh_tmp.addIndices({ n + 4, n + 5, 2 });

	// 一番上の三角柱の背面
	p = 6 * n + 3;
	mesh_tmp.addIndices({ p, n, p + 1 });
	mesh_tmp.addIndices({ p + 1, n + 1, p + 2 });
	mesh_tmp.addIndices({ n, n + 1, p + 1 });
	mesh_tmp.addIndices({ n + 1, n + 2, p + 2 });

	// 側面
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
	// X軸, Y軸，Z軸を描画
	glLineWidth(1.0);
	ofSetColor(ofColor::red);
	ofDrawLine(0, 0, 0, ofGetHeight() / 2, 0, 0);
	ofSetColor(ofColor::green);
	ofDrawLine(0, 0, 0, 0, ofGetHeight() / 2, 0);
	ofSetColor(ofColor::blue);
	ofDrawLine(0, 0, 0, 0, 0, ofGetHeight() / 2);

	ofSetColor(ofColor::gray);				// ポリゴン(面)の色の設定
	mesh.drawFaces();

	ofSetColor(ofColor::blue);			// ポリゴン(稜線)の色の設
	glLineWidth(2.0);
	mesh.drawWireframe();

	ofSetColor(ofColor::white);			// ポリゴン(頂点)の色の設定
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
