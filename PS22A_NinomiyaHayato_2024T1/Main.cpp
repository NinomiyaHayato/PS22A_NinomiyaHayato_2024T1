# include <Siv3D.hpp> // Siv3D v0.6.14


class Ball;


namespace constrants {
	namespace ball {
		constexpr float SPEED = 100.0;

		//constexpr int BALL_RAD = 40;
	}
	namespace brick {
		constexpr Size BRICK_SIZE{ 40, 20 };

		constexpr int X_COUNT = 20;
		constexpr int Y_COUNT = 5;
		constexpr int MAX = X_COUNT * Y_COUNT;

	}
}


class Ball {
public:
	Vec2 ballVec;

	Circle ball;

	Ball() : ballVec({ 0,constrants::ball::SPEED }),ball(400,400,8){}

	void Draw() {
		ball.draw();
	}

	void Update() {
		ball.moveBy(ballVec * Scene::DeltaTime());
	}
};

class Brocks {
public:
	Rect bricks[constrants::brick::MAX];

	void Bricks() {
		using namespace constrants::brick;
		for (int y = 0; y < Y_COUNT; ++y)
		{
			for (int x = 0; x < X_COUNT; ++x)
			{
				int index = y * X_COUNT + x;

				bricks[index] = Rect{
					x * BRICK_SIZE.x,
					60 + y * BRICK_SIZE.y,
					BRICK_SIZE
				};
			}
		}
	}
	void Draw() {
		for (int i = 0; i < constrants::brick::MAX; ++i)
		{
			bricks[i].stretched(-1).draw(HSV{ bricks[i].y - 40 });
		}
	}
	void Intersects(Ball& target) {
		using namespace constrants::brick;
		for (int i = 0; i < MAX; ++i)
		{
			auto& refBrick = bricks[i];

			//ブロックの上辺、また底辺と交差
			if (refBrick.intersects(target.ball))
			{
				if (refBrick.bottom().intersects(target.ball) || refBrick.top().intersects(target.ball))
				{
					target.ballVec.y *= -1;
				}
				else //ブロックの左辺または右辺と交差
				{
					target.ballVec.x *= -1;
				}

				//当たったブロックは画面外
				refBrick.y -= 600;

				//同一フレームでは複数のブロックを検知しない
				break;
			}
		}

		//天井との衝突
		if ((target.ball.y < 0) && (target.ballVec.y < 0))
		{
			target.ballVec.y *= -1;
		}

		//壁との衝突を検知
		if (((target.ball.x < 0) && (target.ballVec.x < 0)) || ((Scene::Width() < target.ball.x) && (0 < target.ballVec.x)))
		{
			target.ballVec.x *= -1;
		}
	}
};

class Paddle {
public:
	Rect paddle;

	Paddle(): paddle{ Arg::center(Cursor::Pos().x, 500),60,10 }{}
	void Draw() {
		paddle.rounded(3).draw();
	}

	void Update() {
		paddle.x = Cursor::Pos().x;
	}

	void Intersects(Ball& target) {
		using namespace	constrants::ball;
		//パドルとの衝突を検知
		if ((0 < target.ballVec.y) && paddle.intersects(target.ball))
		{
			target.ballVec = Vec2{
				(target.ball.x - paddle.center().x) * 10,
				-target.ballVec.y
			}.setLength(SPEED);
		}
	}
};
void Main()
{
	Ball ball;
	Brocks brocks;
	Paddle paddle;

	brocks.Bricks();
#pragma region 更新
	while (System::Update())
	{
		ball.Update();
		paddle.Update();

		ball.Draw();
		brocks.Draw();
		paddle.Draw();

		brocks.Intersects(ball);
		paddle.Intersects(ball);
	};

}


//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
