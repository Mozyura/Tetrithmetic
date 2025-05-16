# include <Siv3D.hpp> // OpenSiv3D v0.6.3

#define LEFT false
#define RIGHT true
#define Empty 0
#define Block 1
#define Unranked -1

constexpr int RANKING_COUNT = 8;
//ブロックの描画サイズ
constexpr int BLOCK_DRAW_SIZE = 25;
//盤面外のブロックの表示サイズ
constexpr int SIDE_BLOCK_DRAW_SIZE = 14;
//テトリミノの長さ　回転の中心を明示する都合上、奇数にすること
constexpr int MINO_SIZE = 5;
//下のバイアス　回転したときに添え字外アクセスが発生しないようにする役割がある
constexpr int FIELD_BIAS = MINO_SIZE - 1;
constexpr int FIELD_X = 12 + FIELD_BIAS * 2;
constexpr int FIELD_Y = 23 + MINO_SIZE + FIELD_BIAS;
constexpr int TETRIS_QUEUE_SIZE = 4;
//タイルの落ち始める場所
constexpr Point START_POINT(FIELD_X / 2, MINO_SIZE / 2);
constexpr int MAX_LEVEL = 5;
constexpr int LEVEL_CEIL = 100;
//暗算の答えの最大桁数
constexpr int MAX_DIGIT = 5;
//落下速度制御 F:強制落下 K:sキーが押されたときの落下速度
constexpr int F_DOWN_CLK = 220;
constexpr int K_DOWN_CLK = 50;
//Sキーを押したときのスコア加点幅
constexpr int S_DOWN_SCORE = 10;
//左右に動くときの速度を制御
constexpr int SIDE_CLK = 80;

//ブロックが設置から確定するまでの最大猶予
constexpr int MAX_EARTING_DURATION = 900;
//ブロックが設置から確定するまでの初期猶予
constexpr int START_EARTING_DURATION = 300;
//ブロックが設置から確定するまでに移動がされた際の
constexpr int ADD_EARTING_DURATION = 200;


//プロトタイプ宣言
struct AppData;
class Ranking;
//エイリアス宣言
using App = SceneManager<String, AppData>;
using MinoPattern = Grid<int>;
using Destractor = std::function<void(void)>;
//jsonから読みだしたデータ
struct MinoData
{
public:
	MinoPattern pattern;
	int color = 0;
	MinoData() = default;
	MinoData(const MinoPattern& p, int c) :pattern(p), color(c) {}
};
//jsonから読みだした設定データ
struct ConfigData
{
public:
	ConfigData() = default;
	int init_level = 2;
	bool level_changeable = true;
	bool tetris_reset_available = false;
	MinoPattern start_tiles = {
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			//ここまで表示されない　かつ　ここがゲームオーバーライン
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
	};

	static ConfigData load()
	{
		const auto filename = U"data/config.json";
		const auto json = JSON::Load(filename);
		ConfigData data;

		if (!json)
		{
			Print << U"{}が見つかりません　通常のゲームモードで進行します"_fmt(filename);
			return ConfigData();
		}
		//設定が有効化されているかどうかを判定
		if (json[U"ActivateConfig"].get<bool>())
		{
			data.init_level = json[U"InitLevel"].get<int>();
			data.level_changeable = json[U"LevelChangeable"].get<bool>();
			data.tetris_reset_available = json[U"TetrisResetAvailable"].get<bool>();

			//テトリミノパターン1つ分のバッファ
			MinoPattern block_shape(FIELD_X, FIELD_Y, Empty);
			//json内のテトリミノ配列インデックスを保存する変数
			int i_column = 0;
			int i_value = 0;
			//ブロックパターンを読み取む
			for (const auto& column : json[U"StartTiles"].arrayView()) {
				i_value = 0;
				for (const auto& value : column.arrayView()) {
					if (value.get<int>() != 0)
					{
						block_shape[i_column][i_value] = 1;
					}
					i_value++;
				}
				i_column++;
			}
			data.start_tiles = block_shape;

			return data;
		}
		else
		{
			return ConfigData();
		}

	}
};
Color transIntToColor(int color)
{
	const auto r = (Color::value_type)(color / 1000000);
	const auto g = (Color::value_type)((color / 1000) % 1000);
	const auto b = (Color::value_type)(color % 1000);
	return Color(r, g, b);
}
int transColorToInt(const Color& color)
{
	return color.r * 1000000 + color.g * 1000 + color.b;
}

//プレイヤーが操作するテトリミノについてのクラス
class Mino {
public:
	Array<Point> blocks;
	Point pos = START_POINT;
	int color = Block;
	int score = 0;

	Mino() = default;
	//2次元配列にマッピングされた形式を引数にとる
	Mino(const MinoPattern& shape) : pos(START_POINT) {
		setShape(shape);
	}
	//テトリミノのセッター  2次元配列にマッピングされた形式を引数にとる
	void setShape(const MinoPattern& shape) {
		blocks.clear();
		//2次元配列の形をブロック座標の配列データに変換
		for (int y = 0; y < MINO_SIZE; y++) {
			for (int x = 0; x < MINO_SIZE; x++) {
				if (shape[y][x] != Empty) {
					blocks << Point{ x - MINO_SIZE / 2, y - MINO_SIZE / 2 };
				}
			}
		}
	}
	void set(const MinoData& data)
	{
		setShape(data.pattern);
		color = data.color;
	}
	MinoPattern getShape() const
	{
		MinoPattern result(MINO_SIZE, MINO_SIZE, Empty);
		for (const auto& block : blocks)
		{
			const Point b_pos = { block.x + MINO_SIZE / 2, block.y + MINO_SIZE / 2 };
			result[b_pos.y][b_pos.x] = color;
		}
		return result;
	}
	//フィールド内でのブロック位置の配列を返す Notテトリミノの形
	Array<Point> MappedBlocks() const {
		return blocks.map([&](const Point& b) {
			return b + pos;
		});
	}
	//回転を行う関数
	void rotate(bool right) {
		//ブロックの各要素に回転行列を適用
		blocks = blocks.map([&](const Point& b) {
			if (right) {
				return Point{ -b.y,b.x };
			}
			else {
				return Point{ b.y,-b.x };
			}
		});
	}
	//描画関数
	void draw() const {
		const auto bds = BLOCK_DRAW_SIZE;
		for (auto& b : blocks) {
			//ここの+1や-1は壁の分
			const Point p = { pos.x + b.x - FIELD_BIAS + 1, pos.y + b.y - FIELD_BIAS - 1 };

			Rect(bds * p.x, bds * p.y, bds)
				.draw(transIntToColor(color))
				.drawFrame(2, Palette::Black);
		}
	}
	MinoData getMinoData() const
	{
		return MinoData(getShape(), color);
	}
};
//テトリスの盤面
class Field {
public:
	Field() = default;
	MinoPattern StartTiles = {
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			//ここまで表示されない　かつ　ここがゲームオーバーライン
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
	};
	//盤面を二次元配列で保持
	MinoPattern tiles = {
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			//ここまで表示されない　かつ　ここがゲームオーバーライン
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} ,
	};
	//フィールドの描画関数
	void draw() const {
		const int bs = BLOCK_DRAW_SIZE;
		//ここでは、添え字外エラーを出すことを防ぐための余分なブロックを描画しないようにしている
		//+1や-1は壁の分
		for (int y = MINO_SIZE; y < FIELD_Y - FIELD_BIAS + 1; y++) {
			for (int x = FIELD_BIAS - 1; x < FIELD_X - FIELD_BIAS + 1; x++) {
				if (tiles[y][x] == Block) {
					Rect((x - FIELD_BIAS + 1) * bs, (y - MINO_SIZE) * bs, bs, bs)
						.draw()
						.drawFrame(2, Palette::Black);
				}
				else if (tiles[y][x] != Empty)
				{
					const Color b_color = transIntToColor(tiles[y][x]);
					Rect((x - FIELD_BIAS + 1) * bs, (y - MINO_SIZE) * bs, bs, bs)
						.draw(b_color)
						.drawFrame(2, Palette::Black);
				}
			}
		}
	}
	void reset()
	{
		tiles = StartTiles;
	}
};

////////////////////////////
//計算に出てくるオペランド
enum class OP {
	add,
	sub,
	mult,
	div,
};

struct ProbremData {
public:
	ProbremData() = default;
	Array<OP> ops;
	Array<int> num1, num2;
	int correct_point = 0, incorrect_point = 0;
	int score = 0;
	double time = 0;
};

struct Probrem {
public:
	int left = 0, right = 0, answer = 0;
	int correct_point = 0, incorrect_point = 0;
	OP op = OP::add;
	Stopwatch time;
	double limit = 0;

	Probrem() = default;

	double getTimeLeft() const
	{
		const double time_left = limit - time.sF();
		if (time_left > 0)
		{
			return time_left;
		}
		else
		{
			return 0;
		}
	}
};
//スコアデータ用の構造体
struct ScoreData
{
public:
	int score = 0;
	String name;
	ScoreData() = default;
	ScoreData(int _score, const String& _name) :score(_score), name(_name) {}

	bool operator==(const ScoreData& s)
	{
		return this->score == s.score && this->name == s.name;
	}
};
//シーン間の共有データ　ファイルから読み込んだデータも格納する
struct AppData {
public:
	//添え字はLv,同Lv内のブロック配列,ブロックパターン
	Array<Array<MinoData>> block_shapes;
	//問題の難易度とポイントデータ 添え字はLv,同レベル内の問題設定、問題データ
	Array<Array<ProbremData>> prob_datas;
	//直前のゲームスコア
	Optional<ScoreData> last_game_score;
	// ハイスコア
	Array<ScoreData> high_scores;

	static AppData load() {
		AppData result;
		//ブロックの形を読み込む プログラム内ではLvは0スタート
		for (int lv = 0; lv < MAX_LEVEL; lv++) {
			//レベルごとのブロックパターン配列
			Array<MinoData> lv_shapes;
			//レベルごとに読み込むためのフォルダ名を生成
			const auto lv_suffix = U"Lv{}"_fmt(lv + 1);
			for (int i = 0; i < 100; i++) {
				//Block00.jsonのようなファイルのサフィックスを生成
				const auto block_suffix = U"Block{:0>2}.json"_fmt(i);
				//jsonファイルの読み込み
				const auto json = JSON::Load(U"data/blocks/{}/{}"_fmt(lv_suffix, block_suffix));
				//ブロックのファイルがこれ以上存在しないとき、レベルごとのループを抜ける
				if (not json) {
					break;
				}
				//テトリミノパターン1つ分のバッファ
				MinoPattern block_shape(5, 5, Empty);
				//json内のテトリミノ配列インデックスを保存する変数
				int i_column = 0;
				int i_value = 0;
				//ブロックパターンを読み取む
				for (const auto& column : json[U"Pattern"].arrayView()) {
					i_value = 0;
					for (const auto& value : column.arrayView()) {
						if (value.get<int>() != 0)
						{
							block_shape[i_column][i_value] = 1;
						}
						i_value++;
					}
					i_column++;
				}
				//色データの読み込み 0~255のRGB
				int color = 0;
				auto c_data = json[U"Color"].arrayView();
				//R
				color += c_data[0].get<int>() * 1000000;
				//G
				color += c_data[1].get<int>() * 1000;
				//B
				color += c_data[2].get<int>();
				//テトリミノのデータを作成
				MinoData m_data(block_shape, color);
				//重みを取得　データがない場合は1で固定する
				try
				{
					const int weight = json[U"Weight"].get<int>();
					for (int a = 0; a < weight; a++)
					{
						lv_shapes << m_data;
					}
				}
				catch (const Error&)
				{
					lv_shapes << m_data;
				}
			}
			result.block_shapes << lv_shapes;
		}
		//問題設定のデータを読み込む　プログラム内ではLvは0スタート
		for (int lv = 0; lv < MAX_LEVEL; lv++) {
			//レベルごとの問題設定データ配列
			Array<ProbremData> lv_probs;
			//レベルごとに読み込むためのフォルダ名を生成
			const auto lv_suffix = U"Lv{}"_fmt(lv + 1);
			for (int i = 0; i < 100; i++) {
				//Probrem00.jsonのようなファイルのサフィックスを生成
				const auto block_suffix = U"Probrem{:0>2}.json"_fmt(i);
				//ファイルネームを格納
				const auto filename = U"data/calc/{}/{}"_fmt(lv_suffix, block_suffix);
				//jsonファイルの読み込み
				const auto json = JSON::Load(filename);
				//ブロックのファイルがこれ以上存在しないとき、レベルごとのループを抜ける
				if (not json) {
					break;
				}
				//問題設定データ1つ分のバッファ
				ProbremData prob_data;
				//使用する演算子を保存
				for (const auto& _op : json[U"op"].arrayView()) {
					const auto& op = _op.get<String>();
					if (op == U"+") {
						prob_data.ops << OP::add;
					}
					else if (op == U"-") {
						prob_data.ops << OP::sub;
					}
					else if (op == U"*") {
						prob_data.ops << OP::mult;
					}
					else if (op == U"/") {
						prob_data.ops << OP::div;
					}
					else {
						Print << U"Illegal operator at {}"_fmt(filename);
					}
				}
				//左辺の桁数を保存
				for (const auto& num1 : json[U"num1"].arrayView()) {
					const auto digit = num1.get<int>();
					if (digit > 0 && digit < MAX_DIGIT) {
						prob_data.num1 << digit;
					}
					else {
						Print << U"Illegal left digit at {}"_fmt(filename);
					}
				}
				//右辺の桁数を保存
				for (const auto& r_digit : json[U"num2"].arrayView()) {
					const auto digit = r_digit.get<int>();
					if (digit > 0 && digit < MAX_DIGIT) {
						prob_data.num2 << digit;
					}
					else {
						Print << U"Illegal right digit at {}"_fmt(filename);
					}
				}
				//正解時の点数の保存
				prob_data.correct_point = json[U"correct_point"].get<int>();
				//不正解時の減少点数の保存
				prob_data.incorrect_point = json[U"incorrect_point"].get<int>();
				//回答時間の保存
				prob_data.time = json[U"time"].get<double>();
				//重みを取得　データがない場合は1で固定する
				try
				{
					const int weight = json[U"Weight"].get<int>();
					for (int a = 0; a < weight; a++)
					{
						lv_probs << prob_data;
					}
				}
				catch (const Error&)
				{
					lv_probs << prob_data;
				}
				lv_probs << prob_data;
			}
			result.prob_datas << lv_probs;
		}
		//スコアの初期化
		{
			const auto filename = U"data/ranking.json";
			const auto json = JSON::Load(filename);
			if (not json)
			{
				throw Error(U"{}が存在しません"_fmt(filename));
			}

			for (int i = 0; i < RANKING_COUNT; i++)
			{
				const int s = json[U"{}"_fmt(i+1)][U"score"].get<int>();
				const String n = json[U"{}"_fmt(i+1)][U"name"].getString();
				result.high_scores << ScoreData(s, n);
			}
			result.high_scores.stable_sort_by([&](const ScoreData& a, const ScoreData& b) {
				return a.score > b.score;
			});
		}

		return result;
	}

	int BlockNum(int lv)const {
		return (int)block_shapes[lv].size();
	}
	int ProbNum(int lv)const {
		return (int)prob_datas[lv].size();
	}
};
//ゲームロジックで使用する時計の識別子
enum class CS {
	TSForcedClk,
	TSSkeyClk,
	TSAkeyClk,
	TSDkeyClk,
	TSEarthingClk,
};
//
enum class STP
{
	Tetris,
	Calc,
	Both,
};

class IStopEffect
{
private:
	STP stp;
	Destractor dest;
protected:
	Stopwatch clock;
public:
	IStopEffect(const STP _stp, const Destractor& _dest) :stp(_stp), dest(_dest)
	{
		clock = Stopwatch(StartImmediately::Yes);
	}
	virtual bool update() = 0;
	virtual void draw() const = 0;
	STP getSTP() const { return stp; }
	void destractor() const
	{
		dest();
	}

};

using STPEffect = std::shared_ptr<IStopEffect>;

class CLCCorrectEffect : public IStopEffect
{
public:
	CLCCorrectEffect(const Destractor& _dest) : IStopEffect(STP::Calc, _dest)
	{

	}

	bool update() override
	{
		return clock.ms() > 800;
	}

	void draw() const override
	{
		Circle(630, 160, 80).drawFrame(15, Palette::Red);
	}
};

class CLCIncorrectEffect : public IStopEffect
{
public:
	CLCIncorrectEffect(const Destractor& _dest) : IStopEffect(STP::Calc, _dest)
	{

	}

	bool update() override
	{
		return clock.ms() > 800;
	}

	void draw() const override
	{
		Shape2D::Cross(80, 10, Vec2{ 630, 160 }).draw(Palette::Blue);
	}
};

class Game : public App::Scene {
private:
	////////共有データ部////////////
	ConfigData config;
	//ゲームロジックの進行状態を制御
	Array<STPEffect> stp_effects;
	//レベルは0スタート(表示上は1から)
	int level = 2;
	//ゲームで時間を伴う制御の際に使用するタイマーを格納したテーブル
	HashTable<CS, Stopwatch> clocks = {
		//テトリミノの強制落下タイミングを制御する
		{CS::TSForcedClk, Stopwatch(StartImmediately::Yes)},
		{CS::TSSkeyClk, Stopwatch(StartImmediately::No)},
		{CS::TSAkeyClk, Stopwatch(StartImmediately::No)},
		{CS::TSDkeyClk, Stopwatch(StartImmediately::No)},
		{CS::TSEarthingClk, Stopwatch(StartImmediately::No)},
	};
	//制御を要求しないエフェクト
	Effect effect;
	//スコアとなる生存時間
	double fall_score_bias = 0;
	Stopwatch living_clock;
	//現在のスコアを算出
	int getScore() const
	{
		return living_clock.sF() * 10 + fall_score_bias;
	}
	//////////テトリス部////////////
	Field field;
	Mino mino;
	MinoData hold;
	Array<MinoData> patterns;
	int e_duration;
	//中心部のパーティクル描画
	ParticleSystem2D particle_system;
	void initParticle()
	{
		//中心のパーティクル描画に関する設定
		particle_system = ParticleSystem2D(mino.pos, { 0,0 });
		//扇形にパーティクル生成
		ArcEmitter2D arcEmitter;
		arcEmitter.angle = 300;
		particle_system.setEmitter(arcEmitter);
		particle_system.setTexture(TextureAsset(U"Particle"));
		//パーティクルの設定
		ParticleSystem2DParameters parameters;
		//表示数を20に
		parameters.rate = 40;
		parameters.startSpeed = 5;
		parameters.startLifeTime = 0.3;
		particle_system.setParameters(parameters);
		particle_system.prewarm();
	}
	//レベルに応じたランダムなテトリミノをキューに入れる
	void generateNextMino() {
		const auto& data = getData();
		const int rand = Random(0, data.BlockNum(level) - 1);

		patterns << data.block_shapes[level][rand];
	}
	//接地かどうか調べる関数
	bool earthing() const {
		for (const auto& block : mino.MappedBlocks()) {
			//テトリミノの下にブロックが存在するか判定
			if (field.tiles[static_cast<size_t>(block.y) + 1][block.x] != Empty) {
				return true;
			}
		}
		return false;
	}
	//テトリミノが盤面と接触していないかどうか判定する関数
	bool overlap() const {
		for (const auto& block : mino.MappedBlocks()) {
			//もし移動後のテトリミノの位置にブロックが存在したら
			if (field.tiles[block.y][block.x] != Empty) {
				return true;
			}
		}
		return false;
	}
	//ブロック設置時に一気に行われる更新処理をひとまとめにしたもの
	void puttingProcess() {
		//フィールドにブロックの設置
		for (const auto& block : mino.MappedBlocks()) {
			field.tiles[block.y][block.x] = mino.color;
		}
		//ブロックの削除処理　かご部分をチェックしないように走査範囲を限定　上から走査
		for (int y = 0; y < FIELD_Y - FIELD_BIAS; y++) {
			int x = FIELD_BIAS;
			const int end = FIELD_X - FIELD_BIAS;
			for (; x < end; x++) {
				if (field.tiles[y][x] == Empty) {
					break;
				}
			}
			//もし一列ブロックで埋まっていたら
			if (x >= end) {
				//下方向に敷き詰める
				for (int w = y; w > 0; w--) {
					for (int v = 1; v < FIELD_X - 1; v++) {
						field.tiles[w][v] = field.tiles[static_cast<size_t>(w) - 1][v];
					}
				}
			}
		}
		//新たなテトリミノの生成
		mino.pos = START_POINT;
		//次のテトリミノパターンを設定
		mino.set(patterns[0]);
		//先頭のパターンを削除し、キューとしての機能を果たす
		patterns.pop_front();
		//テトリミノキューの末尾に追加
		generateNextMino();
	}
	//テトリスの初期化関数
	void setupTetris() {
		living_clock.start();
		field.StartTiles = config.start_tiles;
		field.reset();
		for (int i = 0; i < TETRIS_QUEUE_SIZE + 1; i++) {
			generateNextMino();
		}
		mino.set(patterns[0]);
		patterns.pop_front();
		initParticle();
	}
	//テトリスのアップデート関数
	void updateTetris() {
		//テトリミノの移動幅を保存
		Point m_length = { 0,0 };

		//接地時とそうでないときで入力処理を分岐 これによりTスピンを実現
		if (earthing())
		{
			if (KeyA.down() && KeyD.down()) {}
			//Aキーが押されて左に動くときの動作 接地中は押された瞬間のみを判定
			else if (KeyA.down())
			{
				m_length.x--;
				e_duration += ADD_EARTING_DURATION;
			}
			//Dキーが押されて右に動くときの動作
			else if (KeyD.down())
			{
				m_length.x++;
				e_duration += ADD_EARTING_DURATION;
			}

			//接地時からブロックの設置を確定するまでの猶予時間を制御
			auto& e_clk = clocks[CS::TSEarthingClk];
			//接地時の制御開始を行うための分岐　猶予タイマーを起動
			if (!e_clk.isRunning())
			{
				e_clk.restart();
				e_duration = START_EARTING_DURATION;
			}
			//設置タイマーが最大猶予を越したときに設置させる
			else if (e_clk.ms() > MAX_EARTING_DURATION)
			{
				puttingProcess();
				e_clk.reset();
			}
			//設置タイマーが猶予(キー入力で延長可)を越したときに設置させる
			else if (e_clk.ms() > e_duration)
			{
				puttingProcess();
				e_clk.reset();
			}
		}
		else
			//接地していないときの処理
		{
			auto& forced_clock = clocks[CS::TSForcedClk];
			//AキーとDキーが同時に押されていたら何もしない
			if (KeyA.pressed() && KeyD.pressed()
				|| KeyA.down() && KeyD.down()) {
			}
			//Aキーが押されて左に動くときの動作
			else if (KeyA.down())
			{
				m_length.x--;
				clocks[CS::TSAkeyClk].start();
			}
			else if (KeyA.pressed()) {
				auto& pressed_clock = clocks[CS::TSAkeyClk];
				if (pressed_clock.ms() > SIDE_CLK)
				{
					m_length.x--;
					pressed_clock.restart();
				}
			}
			else if (KeyA.up())
			{
				clocks[CS::TSAkeyClk].reset();
			}
			//Dキーが押されて右に動くときの動作
			else if (KeyD.down())
			{
				m_length.x++;
				clocks[CS::TSDkeyClk].start();
			}
			else if (KeyD.pressed()) {
				auto& pressed_clock = clocks[CS::TSDkeyClk];
				if (pressed_clock.ms() > SIDE_CLK)
				{
					m_length.x++;
					pressed_clock.restart();
				}
			}
			else if (KeyD.up())
			{
				clocks[CS::TSDkeyClk].reset();
			}
			//Sキーを押されたときの落下処理
			if (KeyS.down())
			{
				m_length.y++;
				fall_score_bias += S_DOWN_SCORE;
				clocks[CS::TSSkeyClk].start();
			}
			else if (KeyS.pressed()) {
				auto& down_clock = clocks[CS::TSSkeyClk];
				if (down_clock.ms() > K_DOWN_CLK)
				{
					m_length.y++;
					down_clock.restart();
					fall_score_bias += S_DOWN_SCORE;
					//Sキーが離された瞬間落下するのを防ぐため、一度落下したら強制落下タイマーもリセットする
					clocks[CS::TSForcedClk].restart();
				}
			}
			else if (KeyS.up())
			{
				clocks[CS::TSSkeyClk].reset();
			}
			//強制落下処理
			if (forced_clock.ms() > F_DOWN_CLK) {
				forced_clock.restart();
				m_length.y++;
			}
		}

		//回転は1フレームに最大1回のみ　右優先
		//右回転を行う処理
		if (KeyE.down()) {
			//回転でも接触が発生しないか確認する　そのために最初に回転させる
			mino.rotate(RIGHT);
			//ブロックの重複が存在したら
			if (overlap()) {
				//処理のロールバック
				mino.rotate(LEFT);
			}
			//設置猶予時延長を行う　なおこの猶予時間は接地時に初期化されるため、この値が接地時以外に影響を及ぼすことはない
			e_duration += ADD_EARTING_DURATION;
		}
		//左回転を行う処理
		else if (KeyQ.down()) {
			//こちらでも同様に回転でも接触が発生しないか確認する
			mino.rotate(LEFT);
			//ブロックの重複が存在したら
			if (overlap()) {
				//処理のロールバック
				mino.rotate(RIGHT);
			}
			//設置猶予時延長を行う　なおこの猶予時間は接地時に初期化されるため、この値が接地時以外に影響を及ぼすことはない
			e_duration += ADD_EARTING_DURATION;
		}
		//ホールドを行う処理
		if (KeyW.down())
		{
			//何もホールドされていなければ
			if (hold.pattern.isEmpty())
			{
				//ホールドバッファに現在の形を保存
				hold.pattern = mino.getShape();
				hold.color = mino.color;
				//キューの一番上のテトリミノを設定
				mino.set(patterns[0]);
				//もし盤面と重複するならば
				if (overlap())
				{
					//処理のロールバックを行う
					mino.set(hold);
					hold = MinoData();
				}
				//もし盤面との重複がなければ
				else
				{
					//処理を確定し、テトリミノキューを更新
					patterns.pop_front();
					generateNextMino();
				}
			}
			//何かホールドされているとき
			else
			{
				//現在のテトリミノを一時退避
				const auto temp = mino.getMinoData();
				//ホールドされたテトリミノを設定
				mino.set(hold);
				//もし重複するならば
				if (overlap())
				{
					//処理のロールバックを行う
					mino.set(temp);
				}
				//もし重複が無いならば
				else
				{
					//ホールドバッファに元のテトリミノを代入
					hold = temp;
				}
			}
		}
		//設定有効時に、強制的に盤面を初期化する
		if (KeyR.down() && config.tetris_reset_available)
		{
			field.reset();
		}

		//盤面との接触判定　一度移動させてから判定を行う
		mino.pos += m_length;
		//移動について、もし重複があれば
		if (overlap()) {
			//処理をロールバック
			mino.pos -= m_length;
		}
		//パーティクルの更新
		{
			const int bds = BLOCK_DRAW_SIZE;
			const Point p_pos = { mino.pos.x - FIELD_BIAS + 1, mino.pos.y - FIELD_BIAS - 1 };
			const Point p_draw_pos = p_pos * bds + Point(bds / 2, bds / 2);
			particle_system.setPosition(p_draw_pos);
			particle_system.update();
		}
		//ゲームオーバーの確認処理
		//ゲームオーバーラインの高さ
		const int y = MINO_SIZE - MINO_SIZE / 2;
		for (int x = FIELD_BIAS; x < FIELD_X - FIELD_BIAS; x++)
		{
			if (field.tiles[y][x] != Empty)
			{
				//ゲームオーバーとなった時の処理
				gameover = true;
				//タイマーを止める
				living_clock.pause();
				probrems[0].time.pause();
				//スコアを代入
				auto& data = getData();
				const auto& last_score = ScoreData(getScore(), U"");

				// ランキングを再構成
				data.high_scores << last_score;
				data.high_scores.stable_sort_by([&](const ScoreData& s1, const ScoreData& s2) {return s1.score > s2.score; });
				data.high_scores.resize(RANKING_COUNT);

				// ランクインしていたら m_rank に順位をセット
				for (int32 i = 0; i < RANKING_COUNT; ++i)
				{
					if (data.high_scores[i] == last_score)
					{
						m_rank = i;
						break;
					}
				}

				data.last_game_score = last_score;
				break;
			}
		}
	}
	//テトリスの描画関数
	void drawTetris() const {
		field.draw();
		mino.draw();
		particle_system.draw();

		{
			Transformer2D transformer(Mat3x2::Translate(365, 15));
			Rect(0, 0, 130, 550).drawFrame(2, Palette::Black);

			font(U"NEXT").draw(10, 10, Palette::Black);

			//キュー内のテトリミノの表示
			for (int i = 0; i < patterns.size(); i++) {
				const int bds = SIDE_BLOCK_DRAW_SIZE;
				const auto& pattern = patterns[i];
				const Point offset = { 25,90 * i + 60 };
				for (int y = 0; y < MINO_SIZE; y++) {
					for (int x = 0; x < MINO_SIZE; x++) {
						if (pattern.pattern[y][x] != Empty)
						{
							Rect(offset.x + x * bds, offset.y + y * bds, bds)
								.drawFrame(2, Palette::Black)
								.draw(transIntToColor(pattern.color));
						}
					}
				}
			}

			font(U"HOLD").draw(15, 410, Palette::Black);
			//ホールドバッファが空でないときのみ描画
			if (!hold.pattern.isEmpty())
			{
				const Point offset = { 25, 460 };
				const int bds = SIDE_BLOCK_DRAW_SIZE;
				for (int y = 0; y < MINO_SIZE; y++) {
					for (int x = 0; x < MINO_SIZE; x++) {
						if (hold.pattern[y][x] != Empty)
						{
							Rect(offset.x + x * bds, offset.y + y * bds, bds)
								.drawFrame(2, Palette::Black)
								.draw(transIntToColor(hold.color));
						}
					}
				}
			}

		}
	}

	//////////暗算部/////////////

	//問題を管理する変数
	Array<Probrem> probrems;
	//入力された数値を整数型で保存
	int input = 0;
	//得点を保存
	int point = LEVEL_CEIL / 2;
	void addPoint(const int p)
	{
		point += p;
		if (point < 0)
		{
			if (level <= 0 || not config.level_changeable)
			{
				point = 0;
			}
			else
			{
				point += LEVEL_CEIL;
				level--;
			}
		}
		else if (point >= LEVEL_CEIL)
		{
			if (level >= MAX_LEVEL - 1 || not config.level_changeable)
			{
				point = LEVEL_CEIL;
			}
			else
			{
				point -= LEVEL_CEIL;
				level++;
			}
		}
	}
	//この並びにしておくことによって、入力された数の処理を行いやすくしている
	const Array<Input> num_keys = {
		Key0,Key1,Key2,
		Key3,Key4,Key5,
		Key6,Key7,Key8,Key9,
		KeyNum0,KeyNum1,KeyNum2,
		KeyNum3,KeyNum4,KeyNum5,
		KeyNum6,KeyNum7,KeyNum8,KeyNum9,
	};
	const Font font{ 32, Typeface::Bold };
	//指定された桁数の乱数を生成
	int randNum(int digit) const {
		const Array<char> nonzero_numbers = {
			'1','2','3',
			'4','5','6',
			'7','8','9'
		};
		const Array<char> numbers = {
			'0','1','2','3',
			'4','5','6',
			'7','8','9'
		};
		String result;

		result += nonzero_numbers.choice();
		for (int i = 0; i < digit - 1; i++) {
			result += numbers.choice();
		}

		return Parse<int>(result);
	}
	//問題の生成し、probremsの末尾に追加
	void generateNextProbrem() {
		//問題データ1つ分のバッファ
		Probrem probrem;
		//共有データ読み込み
		const auto& data = getData();
		//ランダムに問題データを抽選
		const int rand = Random(0, data.ProbNum(level) - 1);
		const auto& prob = data.prob_datas[level][rand];
		//問題データのデータを抽選
		probrem.op = prob.ops.choice();
		const auto num1 = randNum(prob.num1.choice());
		const auto num2 = randNum(prob.num2.choice());
		//問題データに沿った問題を生成
		//まず演算子によって分岐する
		switch (probrem.op)
		{
			//加法についてはそのまま左辺、右辺に代入
		case OP::add:
			probrem.left = num1;
			probrem.right = num2;
			probrem.answer = num1 + num2;
			break;
			//減算については、答えが負にならないように分岐させる
		case OP::sub:
			if (num1 >= num2) {
				probrem.left = num1;
				probrem.right = num2;
				probrem.answer = num1 - num2;
			}
			else {
				probrem.left = num2;
				probrem.right = num1;
				probrem.answer = num2 - num1;
			}
			break;
			//掛け算についてはそのまま左辺、右辺に代入
		case OP::mult:
			probrem.left = num1;
			probrem.right = num2;
			probrem.answer = num1 * num2;
			break;
			//割り算は答えを割り切れる数にするために、割る数と答えにnum1,num2を代入	
		case OP::div:
			probrem.right = num1;
			probrem.answer = num2;
			probrem.left = num1 * num2;
			break;
		}
		//答えが5桁を超えてしまうと入力ができなくなるため、どこで発生したかを通知する
		if (probrem.answer / 100000 >= 1) {
			Print << U"Lv{} Probrem{:0>2}.jsonの問題データの計算結果が6桁を超えました";
		}
		//時間に関する内容を代入
		probrem.limit = prob.time;
		probrem.time.restart();
		//問題回答時のポイントを代入
		probrem.correct_point = prob.correct_point;
		probrem.incorrect_point = prob.incorrect_point;

		probrems << probrem;
	}
	//暗算部分のアップデート関数
	void updateCalc()
	{
		//現在解いている問題
		auto& probrem = probrems[0];
		//数字入力を行うための処理
		for (int i = 0; i < num_keys.size(); i++) {
			if (num_keys[i].down())
			{
				//生成される問題は最大5桁までなので、そこを境目として処理を分岐
				if (input < 10000) {
					//桁を1つ増やす
					input *= 10;
					//最下位桁の値を代入
					input += i % 10;
				}
				//既に5桁入力されていれば、最下位桁のみ更新を行う
				else {
					input -= input % 10;
					input += i % 10;
				}
				//入力と答えが一致していたら
				if (input == probrem.answer) {
					//ポイントを与える
					probrem.time.pause();
					input = probrem.answer;
					stp_effects << std::make_shared<CLCCorrectEffect>([&]()
						{
							//新たな問題を作成する
							addPoint(probrem.correct_point);
							generateNextProbrem();
							probrems.pop_front();
							input = 0;
						});
				}
			}
		}
		//バックスペースが入力された際に、桁を減らすための処理
		if (KeyBackspace.down()) {
			input /= 10;
		}
		//時間切れ時の処理
		if (probrem.getTimeLeft() <= 0)
		{
			//減点処理を行う
			input = probrem.answer;
			stp_effects << std::make_shared<CLCIncorrectEffect>([&]() {
				//新たな問題を作成する
				addPoint(-probrem.incorrect_point);
				generateNextProbrem();
				probrems.pop_front();
				input = 0;
			});
		}
	}
	//暗算部分の描画処理
	void calcDraw() const {
		const auto& probrem = probrems[0];
		//演算子に対応した問題文を生成
		String op_str;
		if (probrem.op == OP::add) {
			op_str = U"+";
		}
		else if (probrem.op == OP::sub) {
			op_str = U"-";
		}
		else if (probrem.op == OP::mult) {
			op_str = U"×";
		}
		else if (probrem.op == OP::div) {
			op_str = U"÷";
		}
		const String prob_str =
			U"{:>4} {} {:>4}"_fmt(probrem.left, op_str, probrem.right);

		Rect(10, 10, 280, 550)
			.drawFrame(2, Palette::Saddlebrown);
		font(U"問題").draw(25, 20, ColorF{ 0.1 });
		font(prob_str).draw(40, 60, ColorF{ 0.1 });
		font(U"答え").draw(25, 110, ColorF{ 0.1 });
		font(U"{:>5}"_fmt(input)).draw(80, 150, ColorF{ 0.1 });

		{
			const double percent = (double)point / LEVEL_CEIL;
			font(U"Lv:{}"_fmt(level + 1)).draw(30, 210, ColorF{ 0.1 });
			Rect lv_progress(30, 260, 230, 15);
			lv_progress.drawFrame(3, Palette::Black);
			lv_progress.setSize((int)(lv_progress.w * percent), lv_progress.h).draw(Palette::Blue);
		}

		{
			const double time_left = probrem.getTimeLeft();
			const double percent = time_left / probrem.limit;
			Rect time_progress(30, 350, 230, 15);
			const int length = (int)(time_progress.w * percent);
			font(U"残り {:.1f}秒"_fmt(time_left)).draw(40, 300, Palette::Black);
			time_progress.draw(ColorF{ 0.4 }).drawFrame(3, Palette::Black);
			time_progress.setSize(length, time_progress.h).draw(Color(U"#00c353"));
		}

		{
			font(U"スコア").draw(30, 390, Palette::Black);
			font(U"{}"_fmt(getScore())).draw(30, 450, Palette::Black);
		}
		//
	}

	/////////ゲームオーバー部///////

	//ゲームオーバーかどうか
	bool gameover = false;
	//ボタンとカーソルが重なった時の処理に必要な変数
	Rect m_restartButton{ 540, 300, 230, 60 };
	Transition m_restartTransition{ 0.4s, 0.2s };

	Rect m_rankingButton{ 540, 400, 230, 60 };
	Transition m_rankingTransition{ 0.4s, 0.2s };

	Rect m_exitButton{ 540, 500, 230, 60 };
	Transition m_exitTransition{ 0.4s, 0.2s };

	//入力された名前
	String name;
	//スコアの順位
	int m_rank = Unranked;

	//ゲームオーバー時のマウス入力、キー入力の受付を行う
	void updateGameOver()
	{
		auto& data = getData();

		if (m_rank != Unranked)
		{
			const String tmp_name = name;
			TextInput::UpdateText(name, TextInputMode::AllowBackSpace);
			if (name.size() > 8) {
				name = tmp_name;
			}
			else
			{
				JSON json;
				auto& high_scores = getData().high_scores;
				high_scores[m_rank].name = name;
				//jsonへの書き込み
				for (int i = 0; i < RANKING_COUNT; i++)
				{
					const auto& score = high_scores[i];
					json[U"{}"_fmt(i + 1)][U"score"] = score.score;
					json[U"{}"_fmt(i + 1)][U"name"] = score.name;
				}

				json.save(U"data/ranking.json");
			}
		}

		m_restartTransition.update(m_restartButton.mouseOver());
		m_rankingTransition.update(m_rankingButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_restartButton.mouseOver() || m_rankingButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (m_restartButton.leftClicked())
		{
			//新しいゲームは順位が確定してないため、Unrankedに戻す
			m_rank = Unranked;
			// ゲームシーンへ
			changeScene(U"Game");
		}
		else if (m_rankingButton.leftClicked())
		{
			//新しいゲームは順位が確定してないため、Unrankedに戻す
			m_rank = Unranked;
			// ランキングシーンへ
			changeScene(U"Ranking");
		}
		else if (m_exitButton.leftClicked())
		{
			//新しいゲームは順位が確定してないため、Unrankedに戻す
			m_rank = Unranked;
			// 終了
			changeScene(U"Title");
		}
	}
	//ゲームオーバー時の画面描画
	void drawGameOver() const
	{
		//スコアデータの読み込み
		const auto& data = getData();
		//背景を暗くする描画処理	
		Scene::Rect().draw(Color(0, 0, 0, 150));
		//ゲームオーバー表示
		FontAsset(U"TitleFont")(U"Game Over")
			.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

		//ランクインしたかどうかをを表示
		if (m_rank != Unranked)
		{
			font(U"ランクイン！！").draw(550, 200);
		}
		else
		{
			font(U"ランク外…").draw(550, 200);
		}

		//スコアランキングの描画
		{
			for (int i = 0; i < data.high_scores.size(); i++)
			{
				if (m_rank == i)
				{
					const String editingText = TextInput::GetEditingText();
					const String score_name = name + U"|" + editingText;
					const auto& score = data.high_scores[m_rank].score;
					const String display_text = U"{}. {:＿<8} {}点"_fmt(i+1, score_name, score);
					font(display_text).draw(30, 180 + 50 * i);
				}
				else
				{
					const auto& score = data.high_scores[i];
					const auto display_text = U"{}. {:＿<8} {}点"_fmt(i + 1, score.name, score.score);
					font(display_text).draw(30, 180 + 50 * i);
				}
			}
		}

		//ボタンの描画
		{
			m_restartButton.draw(ColorF{ 1.0, 0.3 * m_restartTransition.value() }).drawFrame(2);
			m_rankingButton.draw(ColorF{ 1.0, 0.3 * m_rankingTransition.value() }).drawFrame(2);
			m_exitButton.draw(ColorF{ 1.0, 0.3 * m_exitTransition.value() }).drawFrame(2);

			FontAsset(U"Menu")(U"RESTART").drawAt(m_restartButton.center(), ColorF{ 0.9 });
			FontAsset(U"Menu")(U"RANKING").drawAt(m_rankingButton.center(), ColorF{ 0.9 });
			FontAsset(U"Menu")(U"EXIT").drawAt(m_exitButton.center(), ColorF{ 0.9 });
		}
	}

public:
	Game(const InitData& init) : IScene{ init }
	{
		// 背景の色を設定 | Set background color
		Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
		//設定ファイルの読み込み
		config = ConfigData::load();
		//初期レベルの設定
		level = config.init_level;
		setupTetris();
		generateNextProbrem();
	}

	void update() override
	{
		if (not gameover)
		{
			if (stp_effects.size() == 0)
			{
				updateCalc();
				updateTetris();
			}
			else
			{
				switch (stp_effects[0]->getSTP())
				{
				case STP::Tetris:
					updateCalc();
					break;
				case STP::Calc:
					updateTetris();
					break;
				default:
					break;
				}
				if (stp_effects[0]->update())
				{
					stp_effects[0]->destractor();
					stp_effects.pop_front();
				}
			}
		}
		else
		{
			updateGameOver();
		}
	}

	void draw() const override
	{
		{
			const Transformer2D transformer{ Mat3x2::Translate(500,5) };
			calcDraw();
		}
		{
			const Transformer2D transformer{ Mat3x2::Translate(8,0) };
			drawTetris();
		}
		for (const auto& stp_effect : stp_effects)
		{
			stp_effect->draw();
		}
		if (gameover)
		{
			drawGameOver();
		}
	}
};

class Title : public App::Scene
{
public:

	Title(const InitData& init)
		: IScene{ init } {}

	void update() override
	{
		m_startTransition.update(m_startButton.mouseOver());
		m_rankingTransition.update(m_rankingButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.mouseOver() || m_rankingButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (m_startButton.leftClicked())
		{
			// ゲームシーンへ
			changeScene(U"Game");
		}
		else if (m_rankingButton.leftClicked())
		{
			// ランキングシーンへ
			changeScene(U"Ranking");
		}
		else if (m_exitButton.leftClicked())
		{
			// 終了
			System::Exit();
		}
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

		FontAsset(U"TitleFont")(U"テトリスメティック")
			.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		m_rankingButton.draw(ColorF{ 1.0, m_rankingTransition.value() }).drawFrame(2);
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

		FontAsset(U"Menu")(U"PLAY").drawAt(m_startButton.center(), ColorF{ 0.25 });
		FontAsset(U"Menu")(U"RANKING").drawAt(m_rankingButton.center(), ColorF{ 0.25 });
		FontAsset(U"Menu")(U"EXIT").drawAt(m_exitButton.center(), ColorF{ 0.25 });
	}

private:

	Rect m_startButton{ Arg::center = Scene::Center(), 300, 60 };
	Transition m_startTransition{ 0.4s, 0.2s };

	Rect m_rankingButton{ Arg::center = Scene::Center().movedBy(0, 100), 300, 60 };
	Transition m_rankingTransition{ 0.4s, 0.2s };

	Rect m_exitButton{ Arg::center = Scene::Center().movedBy(0, 200), 300, 60 };
	Transition m_exitTransition{ 0.4s, 0.2s };
};
// ランキングシーン
class Ranking : public App::Scene
{
public:
	Ranking(const InitData& init)
		: IScene{ init }
	{
		
	}

	void update() override
	{
		if (MouseL.down())
		{
			// タイトルシーンへ
			changeScene(U"Title");
		}
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.2, 0.4, 0.9 });

		FontAsset(U"Ranking")(U"RANKING").drawAt(400, 60);

		const auto& high_scores = getData().high_scores;

		// ランキングを表示
		for (auto i : step(RANKING_COUNT))
		{
			const RectF rect{ 100, 100 + i * 60, 600, 60 };

			rect.draw(ColorF{ 1.0, 1.0 - i * 0.1 });

			const auto displaly_text = U"{}. {:＿<8} {}点"_fmt(i + 1, high_scores[i].name, high_scores[i].score);

			FontAsset(U"Ranking")(displaly_text).draw(rect.pos, ColorF{ 0.25 });
		}
	}
};

void Main()
{
	//ゲームデータの読み込み
	const auto data = std::make_shared<AppData>(AppData::load());
	//フォントの設定
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 30, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
	FontAsset::Register(U"Ranking", 40, Typeface::Heavy);
	FontAsset::Register(U"GameScore", 30, Typeface::Light);
	//パーティクル用のテクスチャ読み込み
	TextureAsset::Register(U"Particle", U"example/particle.png", TextureDesc::Mipped);
	//シーンマネージャーの設定
	App manager(data);
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");
	manager.add<Ranking>(U"Ranking");


	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク | Quick Links =
//
// Siv3D リファレンス
// https://zenn.dev/reputeless/books/siv3d-documentation
//
// Siv3D Reference
// https://zenn.dev/reputeless/books/siv3d-documentation-en
//
// Siv3D コミュニティへの参加
// Slack や Twitter, BBS で気軽に質問や情報交換ができます。
// https://zenn.dev/reputeless/books/siv3d-documentation/viewer/community
//
// Siv3D User Community
// https://zenn.dev/reputeless/books/siv3d-documentation-en/viewer/community
//
// 新機能の提案やバグの報告 | Feedback
// https://github.com/Siv3D/OpenSiv3D/issues
//
