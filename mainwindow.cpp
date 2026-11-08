#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QScreen>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(Ui::MainWindow()), overlayWindow(nullptr), overlayMainLayout(&overlayWindow), overlayLabel(&overlayWindow)
{
    // Setting up UI
    ui.setupUi(this);

    // Making image overlay close when the main window closes. I don't know why it requires false instead of true
    overlayWindow.setAttribute(Qt::WA_QuitOnClose, false);

    // Making image overlay window transparent, so only its contents (image) is visible
    overlayWindow.setAttribute(Qt::WA_TranslucentBackground, true);
    overlayWindow.setStyleSheet("background: transparent");

    // Making image overlay window frameless, transparent for input and always on top
    overlayWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput);

    // Creating image overlay window main layout
    overlayMainLayout.setContentsMargins(0, 0, 0, 0);
    overlayWindow.setLayout(&overlayMainLayout);

    // Creating image overlay label to contain image
    overlayMainLayout.addWidget(&overlayLabel);

    // Default "No map selected"
    ui.chooseMapComboBox->addItem(REALM_PATH_PAIRS[0].second);

    // Populating realms combobox
    for (const auto &el : REALM_PATH_PAIRS)
    {
        ui.chooseRealmComboBox->addItem(el.first);
    }

    // Connecting realm and map selection functionality
    connect(ui.chooseRealmComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::getMaps);
    connect(ui.chooseMapComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::pickMap);

    // Connecting button pressings to their respective functions
    QObject::connect(ui.startShowingButton, &QPushButton::pressed, this, &MainWindow::toggleOverlay);
    QObject::connect(ui.chooseOwnImageButton, &QPushButton::pressed, this, &MainWindow::chooseOwnImage);

    // Setting up X offset slider
    ui.overlayOffsetXSlider->setMinimum(0);
    ui.overlayOffsetXSlider->setMaximum(100);
    QObject::connect(ui.overlayOffsetXSlider, &QSlider::valueChanged, this, &MainWindow::setOverlayOffsetX);

    // Setting up Y offset slider
    ui.overlayOffsetYSlider->setMinimum(0);
    ui.overlayOffsetYSlider->setMaximum(100);
    QObject::connect(ui.overlayOffsetYSlider, &QSlider::valueChanged, this, &MainWindow::setOverlayOffsetY);

    // Setting up Size slider
    ui.overlaySizeSlider->setMinimum(0);
    ui.overlaySizeSlider->setMaximum(100);
    QObject::connect(ui.overlaySizeSlider, &QSlider::valueChanged, this, &MainWindow::setOverlaySize);
    ui.overlaySizeSlider->setValue(overlaySize * 100);

    // Setting up Opacity slider
    ui.overlayOpacitySlider->setMinimum(0);
    ui.overlayOpacitySlider->setMaximum(100);
    QObject::connect(ui.overlayOpacitySlider, &QSlider::valueChanged, this, &MainWindow::setOverlayOpacity);
    ui.overlayOpacitySlider->setValue(overlayOpacity * 100);

    // Moving overlay to the top-left corner
    moveOverlay();
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleOverlay()
{
    // Showing image
    if (overlayWindow.isHidden())
    {
        // Checking if an image is chosen
        if (currentImage.isEmpty())
        {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("No image chosen!");
            msgBox.setText("Please, choose an image first to continue.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }

        // Showing image overlay
        overlayWindow.show();
        ui.startShowingButton->setStyleSheet("background-color: rgb(170, 0, 0); color: rgb(0, 0, 0);");
        ui.startShowingButton->setText("Hide Overlay");

        // Updating image overlay label
        updateImageLabel(overlayLabel);
    } // Hiding image
    else
    {
        overlayWindow.hide();
        ui.startShowingButton->setStyleSheet("background-color: rgb(0, 170, 0); color: rgb(0, 0, 0);");
        ui.startShowingButton->setText("Show Overlay");
    }
}

void MainWindow::resizeOverlay()
{
    if (currentImage.isEmpty())
    {
        return;
    }

    // Getting image size
    const QImageReader reader(currentImage);
    const QSize sizeOfImage = reader.size();
    const int width = sizeOfImage.width();
    const int height = sizeOfImage.height();

    // Resizing overlay
    overlayWindow.setFixedSize(width * overlaySize, height * overlaySize);
    updateImageLabel(overlayLabel);
}

void MainWindow::moveOverlay()
{
    // Getting screen resolution
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int width = screenGeometry.width();
    const int height = screenGeometry.height();

    // Moving image overlay window
    overlayWindow.move(width * overlayOffsetX, height * overlayOffsetY);
}

void MainWindow::getMaps(const int index)
{
    // Clrear previous maps
    ui.chooseMapComboBox->clear();

    // Checking if a realm is selected
    if (index == NO_REALM_CHOSEN_INDEX)
    {
        ui.chooseMapComboBox->addItem(REALM_PATH_PAIRS[0].second);
        return;
    }

    // Getting the path to all maps from this realm
    const QString pathToMaps = buildPath(index);

    // Getting all maps
    const QDir directory(pathToMaps);
    QStringList maps = directory.entryList();

    // Adding them to the combobox
    ui.chooseMapComboBox->addItem(CHOOSE_MAP_MESSAGE);
    for (auto &el : maps)
    {
        el.remove(".png");
        ui.chooseMapComboBox->addItem(el);
    }
}

void MainWindow::pickMap()
{
    // Getting the chosen realm and checking if it's actually selected
    const int realmIndex = ui.chooseRealmComboBox->currentIndex();
    if (realmIndex == NO_REALM_CHOSEN_INDEX)
    {
        return;
    }

    // Getting the chosen map
    const QString chosenMap = ui.chooseMapComboBox->currentText();
    if (chosenMap == CHOOSE_MAP_MESSAGE)
    {
        return;
    }

    // Getting the path to all maps from this realm
    const QString pathToMaps = buildPath(realmIndex);

    // Getting all maps from this realm
    const QDir directory(pathToMaps);
    const QStringList maps = directory.entryList();

    // Finding out which map it is
    for (const auto &el : maps)
    {
        if (el == chosenMap + ".png")
        {
            setImage(pathToMaps + el);
            return;
        }
    }
}

void MainWindow::chooseOwnImage()
{
    // Giving user ability to chose a file
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Images (*.png *.jpeg *.jpg)");

    // Get selected file
    if (dialog.exec())
    {
        const QStringList fileName = dialog.selectedFiles();
        setImage(fileName[0]);
    }
}

void MainWindow::setImage(const QString& url)
{
    // Checking if image is valid
    if (QPixmap(url).isNull())
    {
        QMessageBox::warning(this, "Warning", "Failed to load the image, likely it's corrupted.\n"
                                              "Make sure that it's proper png/jpg/jpeg.\n"
                                              "Simply renaming webp to png for example won't work.");
        return;
    }

    // Setting new image
    currentImage = url;

    // Updating main window label
    updateImageLabel(*ui.mainWindowImage);

    // Resizing image overlay
    resizeOverlay();
}

void MainWindow::setOverlayOffsetX(const int offset)
{
    static const QString overlayOffsetXLabelText = "X offset: ";

    // Updating label
    ui.overlayOffsetXLabel->setText(overlayOffsetXLabelText + QString::number(offset) + "%");

    // Updating overlay
    overlayOffsetX = static_cast<qreal>(offset) / 100;
    moveOverlay();
}

void MainWindow::setOverlayOffsetY(const int offset)
{
    static const QString overlayOffsetYLabelText = "Y offset: ";

    // Updating label
    ui.overlayOffsetYLabel->setText(overlayOffsetYLabelText + QString::number(offset) + "%");

    // Updating overlay
    overlayOffsetY = static_cast<qreal>(offset) / 100;
    moveOverlay();
}

void MainWindow::setOverlaySize(const int size)
{
    static const QString overlaySizeLabelText = "Overlay size: ";

    // Updating label
    ui.overlaySizeLabel->setText(overlaySizeLabelText + QString::number(size) + "%");

    // Updating overlay
    overlaySize = static_cast<qreal>(size) / 100;
    resizeOverlay();
}

void MainWindow::setOverlayOpacity(const int opacity)
{
    static const QString overlayOpacityLabelText = "Overlay opacity: ";

    // Updating label
    ui.overlayOpacityLabel->setText(overlayOpacityLabelText + QString::number(opacity) + "%");

    // Updating overlay
    overlayOpacity = static_cast<qreal>(opacity) / 100;
    overlayWindow.setWindowOpacity(overlayOpacity);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   updateImageLabel(*ui.mainWindowImage);
}

void MainWindow::updateImageLabel(QLabel& label)
{
    if (currentImage.isEmpty())
    {
        return;
    }
    label.setPixmap(QPixmap(currentImage).scaled(label.size(), Qt::KeepAspectRatio));
}

QString MainWindow::buildPath(const int index)
{
    const QString pathToMaps = ":/" + REALM_PATH_PAIRS[index].second + "/resources/Callouts/" + REALM_PATH_PAIRS[index].second + "/";
    return pathToMaps;
}
