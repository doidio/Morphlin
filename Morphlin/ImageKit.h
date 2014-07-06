#ifndef IMAGEKIT_H
#define IMAGEKIT_H

#include "ui_ImageKit.h"

#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkDICOMImageReader.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkExtractVOI.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageHistogramStatistics.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageProperty.h>
#include <vtkImageReslice.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageSliceMapper.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkOutlineFilter.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>

class ImageKit : public QWidget
{
	Q_OBJECT

public:
	ImageKit(QWidget *parent = 0);
	~ImageKit();
	
	// whether ImageKit is initialized
	bool m_bInitialized = false;

	// Link ImageKit to renderWindow
	vtkSmartPointer<vtkRenderer> m_AxialRen;
	vtkSmartPointer<vtkRenderer> m_SagittalRen;
	vtkSmartPointer<vtkRenderer> m_CoronalRen;
	vtkSmartPointer<vtkRenderer> m_SpatialRen;
	
	// Display imageData in 2D sliceViews
	vtkSmartPointer<vtkImageSlice> m_AxialSlice;
	vtkSmartPointer<vtkImageSlice> m_SagittalSlice;
	vtkSmartPointer<vtkImageSlice> m_CoronalSlice;

	vtkSmartPointer<vtkImageSliceMapper> m_AxialMapper;
	vtkSmartPointer<vtkImageSliceMapper> m_SagittalMapper;
	vtkSmartPointer<vtkImageSliceMapper> m_CoronalMapper;

	// Display imageData in 3D sliceViews
	vtkSmartPointer<vtkImagePlaneWidget> m_AxialPlaneWidget;
	vtkSmartPointer<vtkImagePlaneWidget> m_SagittalPlaneWidget;
	vtkSmartPointer<vtkImagePlaneWidget> m_CoronalPlaneWidget;

	// imageData and its property
	vtkSmartPointer<vtkImageData> m_ImageData;
	QString	m_Directory = "";
	QString	m_PatientName = "";
	int		m_ImageDimensions[3];
	int		m_ImageExtent[6];
	double	m_ImageSpacing[3];
	double	m_ImageOrigin[3];
	double	m_ImageBounds[6];
	double	m_ImageCenter[3];
	double	m_ImageScalarRange[2];
	double	m_ImageAutoRange[2];
	double	m_ImageAutoWinLv[2];

public:
	// Read imageData from files
	void ReadDICOMImg(QString Directory);
	void ReadXMLImg(QString FileName);

	// After load m_imageData
	void InitialBasicViews();
	void LoadImageToUI();
	void SetSlicePosition(double positionZ, double positionX, double positionY);
	void SetSliceIndex(int indexZ = -1, int indexX = -1, int indexY = -1);
	void SetWindowLevel(double window, double level);
	void SetUIWindowLevel(double window, double level);

	// Render imageObject
	void SetupRenderer(vtkRenderer* axialRen,
					   vtkRenderer* sagittalRen,
					   vtkRenderer* coronalRen,
					   vtkRenderer* satialRen);

	void Render();

public slots:
	void OnWinLvToGrayscale(int);
	void OnGrayscaleToWinLv(int);
	void OnWinLvPresets(QString);

	// Slots for vtkEvent connection. Tips: SIGNATURE must has no object name
	void OnProgress(vtkObject* caller, unsigned long eventID, void* clientData, void* callData);
	void OnIntStyleImage(vtkObject* caller, unsigned long eventID, void* clientData, void* callData);
	void OnSpatialPlaneWidget(vtkObject* caller, unsigned long eventID, void* clientData, void* callData);

signals:
	void ChangeSliceIndex(int axi, int sag, int cor);

private:
	Ui::ImageKit ui;

	// Turn to FALSE to cut off connection between WinLv and Grayscale
	bool m_bWinLvGrayscaleConnect = true;

	// Connect vtkEvent to QtSlot
	vtkSmartPointer<vtkEventQtSlotConnect> m_VTKQtConnector;

	// Matrices for axial, coronal, sagittal, oblique view orientations
	double m_AxialElements[16];
	double m_SagittalElements[16];
	double m_CoronalElements[16];

	// Presets Window/Level value for common use
	double m_WinLvCTAbdomen[2];
	double m_WinLvCTAir[2];
	double m_WinLvCTBone[2];
	double m_WinLvCTBrain[2];
	double m_WinLvCTLung[2];
	double m_WinLvCTSoftTissue[2];
	double m_WinLvMRI[2];

	// Calculate property for imageData
	void CalcImageInfo();
};

#endif // IMAGEKIT_H
