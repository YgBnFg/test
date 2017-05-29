#include<iostream>
#include<filesystem>
#include <fstream>
#include <thread>
#include <time.h>
#include <windows.h>
#include <chrono>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <functional>
#define pi  std::acos(-1.0)
namespace fs = std::experimental::filesystem;
template<typename T>
std::vector<T> polyfit(const std::vector<T>& oX, const std::vector<T>& oY, int nDegree) {
    using namespace boost::numeric::ublas;

    if (oX.size() != oY.size())
        throw std::invalid_argument("X and Y vector sizes do not match");

    // more intuative this way
    nDegree++;

    int nCount = oX.size();
    matrix<T> oXMatrix(nCount, nDegree);
    matrix<T> oYMatrix(nCount, 1);

    // copy y matrix
    for (int i = 0; i < nCount; i++) {
        oYMatrix(i, 0) = oY[i];
    }

    // create the X matrix
    for (int nRow = 0; nRow < nCount; nRow++) {
        T nVal = 1.0f;
        for (int nCol = 0; nCol < nDegree; nCol++) {
            oXMatrix(nRow, nCol) = nVal;
            nVal *= oX[nRow];
        }
    }

    // transpose X matrix
    matrix<T> oXtMatrix(trans(oXMatrix));
    // multiply transposed X matrix with X matrix
    matrix<T> oXtXMatrix(prec_prod(oXtMatrix, oXMatrix));
    // multiply transposed X matrix with Y matrix
    matrix<T> oXtYMatrix(prec_prod(oXtMatrix, oYMatrix));

    // lu decomposition
    permutation_matrix<int> pert(oXtXMatrix.size1());
    const std::size_t singular = lu_factorize(oXtXMatrix, pert);
    // must be singular
    BOOST_ASSERT(singular == 0);

    // backsubstitution
    lu_substitute(oXtXMatrix, pert, oXtYMatrix);

    // copy the result to coeff
    return std::vector<T>(oXtYMatrix.data().begin(), oXtYMatrix.data().end());
}
template<typename T>
std::vector<T> polyval(const std::vector<T>& oCoeff, const std::vector<T>& oX) {
    int nCount = oX.size();
    int nDegree = oCoeff.size();
    std::vector<T>	oY;

    for (int i = 0; i < nCount; i++) {
        T nY = 0;
        T nXT = 1;
        T nX = oX[i];
        for (int j = 0; j < nDegree; j++) {
            // multiply current x by a coefficient
            nY += oCoeff[j] * nXT;
            // power up the X
            nXT *= nX;
        }
        oY.push_back(nY);
    }
    return oY;
}
void test_lambda() {

}
void test_fs() {
    for (fs::path p : fs::directory_iterator("./"))
        std::cout << p.filename() << '\n';
   

    fs::remove_all("./sb/1");
    
    fs::remove_all("./sb");
}
void test_chrono() {
    auto getNowTime = []() {
        return std::chrono::steady_clock::now();
    };
    auto getTimeDiff = []() {

    };
    //std::chrono::steady_clock::time_point 
    auto t_start = getNowTime();
    Sleep(1001);
    auto t_now = getNowTime();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start);
    std::cout << time_span.count() << std::endl;
}
void test_ploy() {
    std::fstream dtfile;
    dtfile.open("dt.txt", std::ios::in);
    double ddt, xx;
    double sumTime = 0;
    std::vector<double> dt, tx;
    while (!dtfile.eof()) {
        dtfile >> ddt >> xx;
        dt.push_back(ddt);
        tx.push_back(xx);
        std::cout << ddt << " " << xx << std::endl;
        sumTime += ddt;
        dtfile.get();
        if (dtfile.peek() == '/n') break;
    }
    double avgTime = sumTime / dt.size();
    double s = 0;
    for (auto ndt : dt) {
s += (ndt - avgTime) * (ndt - avgTime);
    }
    s /= dt.size();
    std::vector<double> x, y;
    sumTime = 0;
    for (int i = 0; i < dt.size(); i++) {
        if (dt.at(i) <= s) {
            y.push_back(dt.at(i));
            x.push_back(tx.at(i));
            sumTime += dt.at(i);
        }
    }
    double v = (1100 - x[0]) / sumTime;
    if (y.size() > 0) y[0] = 0;
    double nowTime = 0;
    for (auto &ny : y) {
        nowTime += ny;
        ny = nowTime * v + x[0];
    }
    auto cof = polyfit<double>(x, y, 3);
    auto yy = polyval<double>(cof, x);
    for (int i = 0; i < x.size(); i++) {
        std::cout << x[i] << " " << y[i] << " " << yy[i] << std::endl;
    }
}
void test_geometry() {
    std::cout << pi << std::endl;
    std::cout << std::asin(-0.5) / pi * 180 << std::endl;
}
void test_gradient() {

}
void test_bind() {
    using namespace std::placeholders;
    auto f = [](int i, int j) {
        return i > j;
    };
    auto f1 = std::bind(f, _1, 10);

    std::cout << f1(20) << std::endl;
}
void test_recursiveLambda() {
    std::function<int(int, int)> ss;
    ss = [&ss](int i, int j) {
        if (i > j) return 0;
        return i + j + ss(++i, j);
    };
    std::cout << ss(1, 5) << std::endl;
}

//callback
typedef void(__stdcall *p)(int i, int j);
void __stdcall print(int i, int j) {
    std::cout << "call! function point" << i + j << std::endl;
}

bool cmp(int i, int j) {
    return i > j;
}
template<class comppare>
void yyp(comppare cmp) {
    std::cout << "call! template " << cmp(1, 2) << std::endl;
}
void xxp(p a) {
    a(1, 2);
}
class B {
public:
    void b() {
        std::cout << "call!this is class B" << this << std::endl;
    }
    int ab = 1;
};
class A {
public:
    //template<class fun>
    void a(std::function<void()> f) {
        f();
    }
    void fb() {
        fromb();
    }
    std::function<void()> fromb;
};

void test_callBack() {
    xxp(print);
    xxp([](int i, int j) {
        std::cout << "call! lambda " << i + j << std::endl;
    });
    yyp(cmp);
    B bb;
    A aa;
    bb.ab = 0;
    aa.a(std::bind(&B::b, std::ref(bb)));
    aa.a(std::bind(&B::b, &bb));
    aa.a(std::bind(&B::b, bb));
    aa.fromb = std::bind(&B::b, std::ref(bb));
    aa.fb();
}
void test_pointercpy(){
    void *a = static_cast<void*>(new char[10]);
    int *b = (int*)a;
    b[0] = 109000;
    double *c = (double*)a;
    c[1] = 1.123123132;
    void *d = (void*)(new char[10]);
    memcpy(d, a, sizeof(void*)*9);
    std::cout << ((double*)d)[1] << std::endl;

}
void test_union() {
    union UnionType{
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
        float rbg;
    } uu;
    uu.a = uu.g = uu.b = 255; uu.r = 254;
    std::cout << &uu.rbg << std::endl;
    std::cout << (unsigned int)(*reinterpret_cast<unsigned int *>(&uu.rbg)) << std::endl;
    std::cout << (unsigned int)(reinterpret_cast<unsigned char *>(&uu.rbg)[0]) << std::endl;
}
void test_mixBinAStringToFile() {
    std::ofstream writer("a", std::ios::binary | std::ios::out);
    writer << "sdfasdf: "<< " " << 2134 << " ";
    int a = 10;
    writer.write((char *)&a, sizeof(int));
    a = 11;
    writer.write((char *)&a, sizeof(int));
    writer << "zxczxvc" << 2346;
    writer.close();
    std::ifstream reader("a", std::ios::binary | std::ios::in);
    std::string s;
    char c;
    int p;
    reader >> s>>p ;
    std::cout << s << " " << p << std::endl;
    int b;
    reader.read((char *)&b,sizeof(int));
    std::cout << b << std::endl;
    reader.read((char *)&b, sizeof(int));
    std::cout << b << std::endl;
    reader >> s;
    std::cout << s<< std::endl;
    reader.close();
}
void test_struct() {
    struct A {
        float x, y, z;
        float p[3];
    };
    A a;
    float f[] = { 1,2,3,4,5,6 };
    float *aa = &(a.x);
    for (int i = 0; i < 6; i++) {
        aa[i] = f[i];
    }
    std::cout << a.x << " " << a.y << " " << a.z << " " << a.p[0] << " " << a.p[1] << " " << a.p[2] << std::endl;
}
int main() {
    //test_fs();
    //test_chrono();
    //test_ploy();
    //test_lambda();
    //test_geometry();
    //test_gradient();
    //test_bind();
    //test_recursiveLambda();
    //test_callBack();
    //test_pointercpy();
    //test_union();
    //test_mixBinAStringToFile();
    //test_struct();
    //std::cout << __FILE__ << std::endl;
    getchar();
    return 0;

}
