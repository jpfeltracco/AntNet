//
// Created by jpfeltracco on 4/29/18.
//

#ifndef ANTNET_VECTOR2D_H
#define ANTNET_VECTOR2D_H

#include <iostream>

class Vector2D {
public:
    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);

    Vector2D& add(const Vector2D& vec);
    Vector2D& sub(const Vector2D& vec);
    Vector2D& mul(const Vector2D& vec);
    Vector2D& div(const Vector2D& vec);

    friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};


#endif //ANTNET_VECTOR2D_H
