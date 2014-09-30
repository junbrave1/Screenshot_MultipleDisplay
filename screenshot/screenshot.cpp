#include <QtWidgets>
#include "screenshot.h"

//#include <highgui.h>
#include <stdio.h>

uchar Blue[3000][3000];
uchar Green[3000][3000];
uchar Red[3000][3000];
uchar Gray[3000][3000];

//! [0]
Screenshot::Screenshot()
{
    screenshotLabel = new QLabel;
    screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);
    screenshotLabel->setMinimumSize(240, 160);

    createOptionsGroupBox();
    createButtonsLayout();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(screenshotLabel);
    mainLayout->addWidget(optionsGroupBox);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    shootScreen();
    //delaySpinBox->setValue(1);
    setWindowTitle(tr("Screenshot"));
    resize(700, 500);
}
//! [0]

//! [1]
void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    if (!screenshotLabel->pixmap() || scaledSize != screenshotLabel->pixmap()->size())
        updateScreenshotLabel();
}
//! [1]

//! [2]
void Screenshot::newScreenshot()
{
    count = 0;
    /*
    if (hideThisWindowCheckBox->isChecked())
        hide();
    newScreenshotButton->setDisabled(true);

    QTimer::singleShot(delaySpinBox->value() * 1000, this, SLOT(shootScreen()));
    */

    QTimer::singleShot(1, this, SLOT(myTimer()));

}
//! [2]

void Screenshot::myTimer()
{
    // if(count>=10)
      //  return;

    shootScreen();
    /*
    QString format = "png";
    QString fileName = tr("/Users/President/Desktop/test")+QString::number(count)+".png";
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, format.toLatin1().constData());
    count++;
    */
    QTimer::singleShot(100, this, SLOT(myTimer()));
}

//! [3]
/*
void Screenshot::saveScreenshot()
{
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(format.toUpper())
                                                    .arg(format));
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, format.toLatin1().constData());
}
//! [3]
*/
//! [4]
void Screenshot::shootScreen()
{
    /*
    if (delaySpinBox->value() != 0)
        qApp->beep()
*/;
//! [4]
    originalPixmap = QPixmap(); // clear image for low memory situations
                                // on embedded devices.
//! [5]
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        originalPixmap = screen->grabWindow(0);
        changecolor();
    }
    //changecolor();

    updateScreenshotLabel();

    startScreenshotButton->setDisabled(false);
    /*
    if (hideThisWindowCheckBox->isChecked())
        show();
    */
}
/*
//! [5]
void Screenshot::changecolor()
{
    //IplImage *image1;
    image1 = originalPixmap;

    // Load Image RGB Values
    for(int i=0;i<image1->height;i++)
    {
        for(int j=0;j<image1->widthStep;j=j+3)
        {
            Blue[i][(int)(j/3)]=image1->imageData[i*image1->widthStep+j];
            Green[i][(int)(j/3)]=image1->imageData[i*image1->widthStep+j+1];
            Red[i][(int)(j/3)]=image1->imageData[i*image1->widthStep+j+2];
        }
    }

    // Implement Algorithms
    for(int i=0;i<image1->height;i++)
    {
        for(int j=0;j<image1->width;j++)
        {
            Gray[i][j]=(uchar)(0.299*Red[i][j] + 0.587*Green[i][j] + 0.114*Blue[i][j]);
            Red[i][j]=Gray[i][j];
            Green[i][j]=Gray[i][j];
            Blue[i][j]=Gray[i][j];
        }
    }

    // Save Image RGB Values
    for(int i=0;i<image1->height;i++)
    {
        for(int j=0;j<image1->widthStep;j=j+3)
        {
            image1->imageData[i*image1->widthStep+j]=Blue[i][(int)(j/3)];
            image1->imageData[i*image1->widthStep+j+1]=Green[i][(int)(j/3)];
            image1->imageData[i*image1->widthStep+j+2]=Red[i][(int)(j/3)];
        }
    }

}
*/
//! [6]

//! [6]
void Screenshot::changecolor()
{
    QImage image = originalPixmap.toImage();
    QRgb col;
    int gray;
    int width = originalPixmap.width();
    int height = originalPixmap.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            col = image.pixel(i, j);
            gray = qGray(col);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    originalPixmap = originalPixmap.fromImage(image);
    updateScreenshotLabel();
}
//! [7]


void Screenshot::createOptionsGroupBox()
{
    optionsGroupBox = new QGroupBox(tr("Options"));

    SpinBox = new QSpinBox;
    SpinBox->setMinimum(0);
    SpinBox->setMaximum(10);
    SpinBox->setValue(2);
    connect(SpinBox, SIGNAL(valueChanged(int)), this, SLOT(correctRange));

    SpinBoxLabel = new QLabel(tr("Correction Range:"));

    optionsGroupBoxLayout = new QGridLayout;
    optionsGroupBoxLayout->addWidget(SpinBoxLabel, 0, 0);
    optionsGroupBoxLayout->addWidget(SpinBox, 0, 1);
    optionsGroupBox->setLayout(optionsGroupBoxLayout);
}

//! [7]

//! [8]
void Screenshot::createButtonsLayout()
{
    startScreenshotButton = createButton(tr("Start Screenshot"), this, SLOT(newScreenshot()));
    //saveScreenshotButton = createButton(tr("Save Screenshot"), this, SLOT(saveScreenshot()));
    quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(startScreenshotButton);
    //buttonsLayout->addWidget(saveScreenshotButton);
    buttonsLayout->addWidget(quitScreenshotButton);
}
//! [8]

//! [9]
QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->connect(button, SIGNAL(clicked()), receiver, member);
    return button;
}
//! [9]

//! [10]
void Screenshot::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}
//! [10]
