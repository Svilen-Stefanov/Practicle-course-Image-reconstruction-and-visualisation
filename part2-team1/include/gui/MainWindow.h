#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>
#include <math/Volume.h>
#include <gui/AcquisitionWidget.h>
#include <gui/ReconstructionWidget.h>
#include <gui/VisualizationWidget.h>

class MainWindow : public QMainWindow {

	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

public slots:
	void load();
	void load(QString file);
	void debugVolume();

signals:
	void loadedFileCorrectly(Volume_ptr vol);

private:
	Volume_ptr volume_;
	QTabWidget* tabWidget_;
	QSettings settings_;

	//the widgets need to be declared here to make the connections in connectSlots
	AcquisitionWidget* acquisition_;
	ReconstructionWidget* reconstruction_;
	VisualizationWidget* visualization_;

	void connectSlots();
	void centerWindow();
	bool isFileLoaded(const QString &fileName);
	void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
};



#endif
