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
#include <opencv2/opencv.hpp>
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
class sss {
public:
    std::string ss = "xxxx";
    const char* cstr() {
        std::cout << ss << std::endl;
        std::cout << ss.c_str() << std::endl;

        return ss.c_str();
    }
};
void test_cstr() {
    sss l;
    std::string c (l.cstr());
    
    std::cout << c<< std::endl;
}
void ff(char **c) {
    for (int i = 0; i < 10; i++) {
        strcpy(c[i],"1111");
    }
}
void test_funxx() {

}
void test_voidx() {
    void *data = (void*)new USHORT[100]{1};
    for (int i = 0; i < 10; i++) {
        std::cout << ((USHORT*)data)[i] << std::endl;
    }
}
#include<stdio.h>  
#include<stdarg.h>  
void m_print(const char *fmt,...) {
    va_list argptr;
    va_start(argptr, fmt);
    char buff[110100];
    int cnt = vsprintf(buff, fmt, argptr);
    printf("%s\n", buff);
    va_end(argptr);
}
#define PT(info, ...)  m_print(info, __VA_ARGS__)
void test_Arbitrary_Argument_Lists() {
    PT("%d %s",1,"xxxx");
}
#define CLR(a,b) std::memset((a),(b),sizeof((a)))  
#define EPS std::numeric_limits<double>::epsilon()
typedef long long ll;
double a[35][35];
double x[35];

int gauss(int var, int equ) {
    int k, col, max_r;
    for (col = 0, k = 0; k < equ&&col < var; k++, col++) {
        max_r = k;
        for (int i = k + 1; i < equ; i++)
            if (fabs(a[i][col]) - fabs(a[max_r][col]) > EPS) max_r = i;
        if (max_r != k)
            for (int j = col; j <= var; j++) std::swap(a[k][j], a[max_r][j]);
        if (fabs(a[k][col]) <= EPS) {
            k--;
            continue;
        }
        for (int i = k + 1; i < equ; i++) {
            if (fabs(a[i][col]) > EPS) {
                double t = a[i][col] / a[k][col];
                //a[i][col] = 0.0;

                for (int j = col; j <= var; j++) {
                    a[i][j] -=  a[k][j] * t;
                }
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 13; j++) {
                printf("%.4f ", a[i][j]);
            }
            std::cout << std::endl;
        }
    }
    
    for (int i = k; i<equ; i++) {
        if (fabs(a[i][col]) <= EPS) return -1;
    }
    if (k<var) return var - k;
    for (int i = var - 1; i >= 0; i--) {
        int tmp = a[i][var];
        for (int j = i + 1; j<var; j++) {
            if (fabs(a[i][j]) > EPS) tmp = tmp - a[i][j] * x[j];
        }
        x[i] = tmp / a[i][i];
    }
}

void test_guass() {
    double fx = 1438.041142533069;
    double fy = 1433.599028322583;
    double ppx = 661.4346053463350;
    double ppy = 327.3481417825431;
    double _a[30][5] = {

    {0.592565, 0.04931, -0.0690865},

    {0.582337, -0.0213424, -0.0304872 },

    {0.688848, -0.0356948, -0.192646 },
    {0.648726, 0.0513119 ,-0.0440199 },
    {0.679401, -0.100948 ,-0.156996},
    {0.639279, -0.0139414, -0.00837033},

    { 0.387897,  -0.00920812,  -0.0158922 },

    { 0.407136,  -0.0764514,  0.025272 },
    { 0.400357,  -0.0407988,  -0.0672067 },
    { 0.561747, 0.0244269,  -0.0360878},

    { 0.418126,   -0.102903,  -0.0291881 },
    { 0.4358,   -0.0266858 , -0.107272 },

    { 0.467745,   0.0301496 ,  0.0368793 },
    { 0.480101,   0.00222315 , -0.138707},
    { 0.515805,   0.0657483 , 0.0224124},
    { 0.529316,   -0.0532374,  -0.127746 },
    { 0.565021,   0.0102878,  0.0333728, }
    };
    double pix[17][2] = {

        {648, 262},

        {735, 430},

        {218, 448},
        {560, 250},
        {310, 651},
        {659, 463},
        {539, 555},

        {703, 806},
 

        {791, 388},
        {970, 686},
        {862, 218},

        {286, 404},
        {854, 192},

        {365, 534},
        {942, 343},
        {466, 743},
        {943, 536},
    };
    
    for (int i = 0; i < 17; i++) {
        double u = pix[i][0];
        double v = pix[i][1];
        for (int equr = 0; equr < 3; equr++) {
            for (int j = 0; j < 4; j++) {
                a[i][equr * 4 + j] = _a[i][j];
                if (equr == 2) {
                    a[i][equr * 4 + j] *= ((ppx - u)/fx + (ppy - v) / fy);
                }
            }
            
        }
        a[i][12] = 0;
        for (int j = 0; j < 13; j++) {
            printf("%.4f ",a[i][j]);
        }
        std::cout << std::endl;
    }
    
    std::cout << gauss(12,17) << std::endl; 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.4f ", x[i * 4 + j]);
        }
        std::cout << std::endl;
    }
    return;
}
void test_pnp() {

    float *_k = new float[12]{
        2415.63711384027f, 0,1078.03363956878f,
        0,2391.72637967550f,518.803152882631f,
        0,0,1
    };
    //float *_k = new float[12]{
    //    1438.041142533069f, 0,661.4346053463350f,
    //    0,1433.599028322583f,327.3481417825431f,
    //    0,0,1
    //};
    float *_a = new float[200]{

         /*-0.124591f, -0.362184f, -1.38539f ,
         0.0324228f, -0.380973f, -1.38048f ,
         -0.128516f, -0.411743f, -1.44953f ,
         0.0284978f, -0.430531f, -1.44462f ,
         -0.127128f, -0.315411f, -1.42529f ,
         0.0483665f, -0.336412f, -1.4198f,*/

          //-0.145569f, -0.234125f, -1.48086f ,
          //-0.00536409f, -0.230123f, -1.40766f ,
          //-0.132232f, -0.311284f, -1.50218f,
          //0.00797232f, -0.307282f, -1.42899f ,

          //0.0297118f, -0.21407f, -1.45561f ,
          //0.042029f, -0.285333f, -1.4753f ,

          0.0179604f, -0.190724f, -1.62964f,
          0.175589f, -0.196659f, -1.61746f,
          0.0162279f, -0.270206f, -1.64595f,
          0.173857f, -0.27614f, -1.63377f,

        -0.0307706f, -0.143788f, -1.4861f,
        0.127415f, -0.141683f, -1.48792f,
        -0.0300192f, -0.221245f, -1.51031f,
        0.128166f, -0.21914f, -1.51213f,


    };
    float *pix = new float[100] {
                
      /*  835, 664,
        1160, 705,
        839, 761,
        1145, 801,
        843, 570,
        1187, 605,*/

                 

        //728, 439,
        //993, 412,
        //769, 582,
        //1029, 559,

        //1059, 382,

        //1089, 515,

        1078, 348,
        1352, 350,
        1081, 484,
        1352, 486,
        977, 225,
        1288, 220,
        988 ,375,
        1283,372


    };
    int pointcnt = 8;
    cv::Mat k(3, 3, CV_32FC1,(void*)_k);
    cv::Mat point3Dset(pointcnt,3,CV_32FC1,(void*)_a);
    cv::Mat point2Dset(pointcnt, 2, CV_32FC1, (void*)pix);
    cv::Mat rvec(3,3,CV_32FC1);
    cv::Mat tvec(3,1,CV_32FC1);
    std::cout << k << std::endl << point3Dset << std::endl << point2Dset << std::endl << rvec << std::endl<< tvec << std::endl;
    try {
        cv::solvePnPRansac(point3Dset, point2Dset, k, cv::Mat(), rvec, tvec);
        cv::Rodrigues(rvec, rvec);
    }
    catch (...) {
        std::cout <<"xx" << std::endl;
    }
    for (int i = 0; i < 3; i++) {
        std::cout << "[";
        for (int j = 0; j < 3; j++) {
            std::cout << rvec.at<double>(i, j)<<", ";
        }
        std::cout <<  tvec.at<double>(i, 0)<<"]" << std::endl;
    }
    std::cout << rvec << std::endl;
    std::cout << tvec << std::endl;
}
void test_vec() {
    /*std::vector<cv::Vec3d> a{cv::Vec3d(1.0,2.0,3.0),cv::Vec3d(1.0,2.0,3.0) };
    double *b = a.data()->val;
    for (int i = 0; i < 6; i++) {
        std::cout << b[i] << std::endl;
    }*/

    std::vector<int> ap{0,1,2,3,4,5};
    int x = 100;
    std::cout << ap.size() << std::endl;
    std::cout << (int)ap.size() - x << std::endl;
   /* ap.erase(ap.begin());
    for (auto x : ap) {
        std::cout << x << std::endl;
    }*/
}

void test_video_capture() {
    cv::Mat frame;
    cv::VideoCapture cap("C:/Users/binfeng.yang/Desktop/output/20170802_175446.avi");
    if (!cap.isOpened()) {
        std::cout << "open fail!" << std::endl;
    }
    int count = cap.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout << count << std::endl;
    while (1) {
        int fid;
        std::cin >> fid;
        if (fid >= count) continue;
        cap.set(CV_CAP_PROP_POS_FRAMES,fid);
        while (cap.read(frame)) {
            cv::imshow("xx", frame);
            if(cv::waitKey(1) == 27) break;
        }
    }
}

int main(int argc, char** argv) {
    printf("%s\n", argv[0]);
    
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
    //test_cstr();
    //test_Arbitrary_Argument_Lists();
    //test_funxx();
    //test_voidx();
    //test_guass();
    //test_pnp();
    test_vec();
    //test_video_capture();
    getchar();
    return 0;

}
