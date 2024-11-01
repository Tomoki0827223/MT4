#include <Novice.h>
#include <cmath>
#include <assert.h>

const char kWindowTitle[] = "LE2D_19_ムラタ_トモキ";

struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};


float Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


float Length(const Vector3& v) {
    return std::sqrt(Dot(v, v));
}


Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}


Vector3 Normalize(const Vector3& v) {
    float length = Length(v);
    assert(length != 0.0f);
    return { v.x / length, v.y / length, v.z / length };
}

// 方向を別の方向へ回転させる行列を生成する関数
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {


    Vector3 cross = Cross(from, to);
    float cos = Dot(from, to);
    float sin = Length(cross);

    float epsilon = 1e-6f;
    Vector3 axis = {};
    if (std::abs(cos + 1.0f) <= epsilon) {


        if (std::abs(from.x) > epsilon || std::abs(from.y) > epsilon) {
            axis = { -from.y, from.x, 0.0f };
        }
        else {
            axis = { 0.0f, -from.z, from.y };
        }
        axis = Normalize(axis);
    }
    else {
        axis = Normalize(cross);
    }

    // 回転行列を構成する
    Matrix4x4 rotateMatrix = {};
    float oneMinusCos = 1.0f - cos;

    rotateMatrix.m[0][0] = cos + axis.x * axis.x * oneMinusCos;
    rotateMatrix.m[1][0] = axis.x * axis.y * oneMinusCos - axis.z * sin;
    rotateMatrix.m[2][0] = axis.x * axis.z * oneMinusCos + axis.y * sin;
    rotateMatrix.m[3][0] = 0.0f;

    rotateMatrix.m[0][1] = axis.y * axis.x * oneMinusCos + axis.z * sin;
    rotateMatrix.m[1][1] = cos + axis.y * axis.y * oneMinusCos;
    rotateMatrix.m[2][1] = axis.y * axis.z * oneMinusCos - axis.x * sin;
    rotateMatrix.m[3][1] = 0.0f;

    rotateMatrix.m[0][2] = axis.z * axis.x * oneMinusCos - axis.y * sin;
    rotateMatrix.m[1][2] = axis.z * axis.y * oneMinusCos + axis.x * sin;
    rotateMatrix.m[2][2] = cos + axis.z * axis.z * oneMinusCos;
    rotateMatrix.m[3][2] = 0.0f;

    rotateMatrix.m[0][3] = 0.0f;
    rotateMatrix.m[1][3] = 0.0f;
    rotateMatrix.m[2][3] = 0.0f;
    rotateMatrix.m[3][3] = 1.0f;

    return rotateMatrix;
}


static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            Novice::ScreenPrintf(
                x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f",
                matrix.m[row][column]);
        }
    }
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    const int kWindowWidth = 1280;
    const int kWindowHeight = 720;
    Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Vector3 v1{ 1.0f, 3.0f, -5.0f };
    Vector3 v2{ 4.0f, -1.0f, 2.0f };
    //float k = { 4.0f };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        Vector3 from0 = Normalize(Vector3{ 1.0f, 0.7f, 0.5f });
        Vector3 to0 = { -from0.x, -from0.y, -from0.z };
        Vector3 from1 = Normalize(Vector3{ -0.6f, 0.9f, 0.2f });
        Vector3 to1 = Normalize(Vector3{ 0.4f, 0.7f, -0.5f });
        Matrix4x4 rotateMatrix0 = DirectionToDirection(
            Normalize(Vector3{ 1.0f, 0.0f, 0.0f }), Normalize(Vector3{ -1.0f, 0.0f, 0.0f }));
        Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
        Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
        MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
        MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
