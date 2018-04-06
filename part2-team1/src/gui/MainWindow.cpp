#include <QtWidgets>
#include <QColor>
#include <iostream>
#include <QDebug>

#include <gui/MainWindow.h>
#include <gui/AcquisitionWidget.h>
#include <io/EDFhandler.h>

MainWindow::MainWindow() {
	acquisition_ = new AcquisitionWidget();
	reconstruction_ = new ReconstructionWidget();
	visualization_ = new VisualizationWidget();

	QToolBar *fileToolBar = addToolBar(tr("File"));
	fileToolBar->setFloatable(false);

	//load
	QAction *loadAction = fileToolBar->addAction(QIcon::fromTheme("document-open"), "&Load");
	loadAction->setShortcuts(QKeySequence::Open);
	loadAction->setStatusTip(tr("Load"));
	connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

	//TABS
	tabWidget_ = new QTabWidget;
	tabWidget_->addTab(acquisition_, tr("Acquistion"));
	tabWidget_->addTab(reconstruction_, tr("Reconstruction"));
	tabWidget_->addTab(visualization_, tr("Visualization"));
	//tabWidget_->addTab(); // for visualization

	QWidget* central = new QWidget(this);
	setCentralWidget(central);

	//main layout
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(tabWidget_);
    central->setLayout(mainLayout);
    setWindowTitle(tr("BasiCT"));

	// resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
	centerWindow();
	connectSlots();


}

MainWindow::~MainWindow() {

}

void MainWindow::connectSlots() {
	//connect all signals and slots here for transferring data between TABS

	//volume loading
	connect(this, &MainWindow::loadedFileCorrectly, acquisition_, &AcquisitionWidget::setVolume);
	connect(this, &MainWindow::loadedFileCorrectly, visualization_, &VisualizationWidget::setVolume);

	//connect acquisition to reconstruction
	connect(acquisition_, SIGNAL(sendAcquisitionPoses(const AcquisitionPoses* const)),
		reconstruction_, SLOT(setAcquisitionPoses(const AcquisitionPoses* const)));
	connect(acquisition_, &AcquisitionWidget::sendAcquisitionPoses,
		[this](const AcquisitionPoses* const p) {this->tabWidget_->setCurrentIndex(1); }); //focus reconstruction tab

	//connect reconstruction to visualization
	connect(reconstruction_, &ReconstructionWidget::sendReconstructedVolume,
		visualization_, &VisualizationWidget::setVolume);
}

void MainWindow::initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
	QString path = settings_.value("default_dir", "").toString();
	dialog.setDirectory(path);

	dialog.setNameFilter("*.edf");

	if (acceptMode == QFileDialog::AcceptSave)
		dialog.setDefaultSuffix("edf");
}

 /*Main function for loading a file*/
void MainWindow::load(){
	QFileDialog dialog(this, tr("Open File"));
	initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
	if (dialog.exec() == QDialog::Accepted)
	{
		load(dialog.selectedFiles().first());
	}
}

void MainWindow::load(QString file)
{
	qDebug() << "Load " << file;
	if (isFileLoaded(file)) {
		//loaded
		QString dir = QFileInfo(file).absoluteDir().absolutePath();
		settings_.setValue("default_dir", dir);
#if defined(DEBUG) || defined(_DEBUG)
		setWindowTitle(QString().sprintf("BasiCT - %s (DEBUG)", QFileInfo(file).fileName().toStdString().c_str()));
#else
		setWindowTitle(QString().sprintf("BasiCT - %s", QFileInfo(file).fileName().toStdString().c_str()));
#endif
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("Unable to load volume");
		msgBox.setIcon(QMessageBox::Icon::Critical);
		msgBox.exec();
	}
}

void MainWindow::debugVolume()
{
	if (volume_ == nullptr)
	{
		qDebug() << "no volume loaded";
	}
	Eigen::Vector3i res = volume_->getNumVoxels();
	//first pass, find min and max values
	float min = std::numeric_limits<float>::max();
	float max = -std::numeric_limits<float>::min();
	for (int z = 0; z < res.z(); ++z) {
		for (int y = 0; y < res.y(); ++y) {
			for (int x = 0; x < res.x(); ++x) {
				float value = volume_->getVoxel(x, y, z);
				min = std::min(min, value);
				max = std::max(max, value);
			}
		}
	}
	//second pass, write files
	for (int z = 0; z < res.z(); ++z) {
		QImage img(res.x(), res.y(), QImage::Format_RGB888);
		for (int y = 0; y < res.y(); ++y) {
			for (int x = 0; x < res.x(); ++x) {
				float value = volume_->getVoxel(x, y, z);
				value = (value - min) / (max - min); //clamp to [0,1]
				int valI = (int)(value * 255);
				img.setPixel(x, y, QColor(valI, valI, valI).rgb());
			}
		}
		img = img.scaled(res.x() * 10, res.y() * 10, Qt::IgnoreAspectRatio, Qt::TransformationMode::FastTransformation);
		img.save(QString::fromStdString("slice" + std::to_string(z) + ".png"), "png");
	}
	qDebug() << "slices saved";
}

/*Must check whether the loading of the file was successful and should set initial values*/
bool MainWindow::isFileLoaded(const QString &fileName){
	std::string name = fileName.toUtf8().constData();
	volume_ = EDFHandler::read(name);

	//if loading didn't work
	if(!volume_)
		return false;
	emit(loadedFileCorrectly(volume_));
	return true;
}

void MainWindow::centerWindow() {
  this->setGeometry(
    QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        this->size(),
        qApp->desktop()->availableGeometry()
    )
  );
}
