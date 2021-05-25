 #ifndef CAMERAFORM_H
#define CAMERAFORM_H

#include <QWidget>

namespace Ui
{
class CameraForm;
}

class CameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit CameraForm(QWidget *parent = nullptr);
    ~CameraForm();

private:
    Ui::CameraForm *ui;
};

#endif // CAMERAFORM_H
