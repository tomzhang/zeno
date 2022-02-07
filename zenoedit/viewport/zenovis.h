#ifndef __ZENOVIS_H__
#define __ZENOVIS_H__

#include <QtWidgets>

class CameraControl;
class CameraKeyframeWidget;

struct PerspectiveInfo
{
    double cx;
    double cy;
    double cz;
    double theta;
    double phi;
    double radius;
    double fov;
    bool ortho_mode;
    PerspectiveInfo() : cx(0), cy(0), cz(0), theta(0), phi(0), radius(0), fov(0), ortho_mode(0) {}
    PerspectiveInfo(double cx, double cy, double cz, double theta, double phi, double radius, double fov, bool ortho_mode)
        : cx(cx), cy(cy), cz(cz), theta(theta), phi(phi), radius(radius), fov(fov), ortho_mode(ortho_mode) {}
};

class Zenovis : public QObject
{
    Q_OBJECT
    typedef std::tuple<QString, QString, QString> FRAME_FILE;

signals:
    void frameUpdated(int);

public slots:
    void startPlay(bool bPlaying);
    int setCurrentFrameId(int frameid);

public:
    static Zenovis& GetInstance();
    void initializeGL();
    void paintGL();
    void recordGL(const std::string& record_path);
    int getCurrentFrameId();

//private:
    Zenovis();
    void _uploadStatus();
    void _recieveStatus();
    void _frameUpdate();

    QList<FRAME_FILE> getFrameFiles(int frameid);
    int getFrameCount(int* max_frameid = nullptr);
    static QString sIoPath;

    int m_solver_frameid;
    int m_solver_interval;
    int m_render_fps;
    QVector2D m_resolution;
    PerspectiveInfo m_perspective;
    int m_cache_frames;
    bool m_show_grid;
    bool m_playing;
    CameraKeyframeWidget* m_camera_keyframe;
    std::shared_ptr<CameraControl> m_camera_control;
    QList<FRAME_FILE> m_frame_files;
};

#endif
