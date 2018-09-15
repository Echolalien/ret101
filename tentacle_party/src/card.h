#ifndef CARD_H
#define CARD_H


class card {

    public:
        card();
        void setup(float xp, float yp, float d, float spd);
        void update();
        void draw();

        float x, y;
        bool reveal, lock;
        float toBlack;
        float difficulty;
        float speed;
        bool touch;
        float progress;
};

#endif // CARD_H
