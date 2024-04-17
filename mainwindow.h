#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Main Window Constructor
    MainWindow(QWidget *parent = nullptr);

    // Cleanup
    ~MainWindow();
public slots:
    // Shows or hides image overlay
    void toggleOverlay();

    // Resizes image overlay
    void resizeOverlay();

    // Moves image overlay
    void moveOverlay();

    // Helper function, which loads maps to the Maps Combo Box after a realm was chosen
    void getMaps(const int index);

    // Picks currently chosen map in the Maps Combo Box
    void pickMap();

    // Loads user-picked image
    void chooseOwnImage();

    // Sets chosen image
    void setImage(const QString url);

    // Sets image overlay X position
    void setOverlayOffsetX(const int offset);

    // Sets image overlay Y position
    void setOverlayOffsetY(const int offset);

    // Sets image overlay size
    void setOverlaySize(const int size);

    // Sets image overlay opacity
    void setOverlayOpacity(const int opacity);
protected:
    // Resizes image label in main window in case of main window resize
    void resizeEvent(QResizeEvent *event) override;
private:
    // Updates image on this label
    void updateImageLabel(QLabel *label);

    // Builds path to maps
    QString buildPath(const int index);

    // Main ui object
    Ui::MainWindow *ui;

    // Overlay window
    QWidget *overlayWindow = nullptr;

    // Overlay label, which contains image
    QLabel *overlayLabel = nullptr;

    // Overlay opacity, can be between 0.0 and 1.0
    qreal overlayOpacity = 1.0;

    // Overlay size, can be between 0.0 and 1.0
    qreal overlaySize = 0.1;

    // Overlay offset by x, can be between 0.0 and 1.0
    qreal overlayOffsetX = 0.0;

    // Overlay offset by y, can be between 0.0 and 1.0
    qreal overlayOffsetY = 0.0;

    // Current overlay image
    QString currentImage = "";
};
#endif // MAINWINDOW_H
