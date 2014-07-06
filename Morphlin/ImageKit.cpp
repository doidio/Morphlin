#include "ImageKit.h"

ImageKit::ImageKit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.ProgressBar->hide();

	m_AxialRen = vtkSmartPointer<vtkRenderer>::New();
	m_SagittalRen = vtkSmartPointer<vtkRenderer>::New();
	m_CoronalRen = vtkSmartPointer<vtkRenderer>::New();
	m_SpatialRen = vtkSmartPointer<vtkRenderer>::New();

	m_AxialSlice = vtkSmartPointer<vtkImageSlice>::New();
	m_SagittalSlice = vtkSmartPointer<vtkImageSlice>::New();
	m_CoronalSlice = vtkSmartPointer<vtkImageSlice>::New();
	m_AxialMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
	m_SagittalMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
	m_CoronalMapper = vtkSmartPointer<vtkImageSliceMapper>::New();

	m_AxialPlaneWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
	m_SagittalPlaneWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
	m_CoronalPlaneWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();

	m_ImageData = vtkSmartPointer<vtkImageData>::New();

	m_VTKQtConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();

	m_AxialElements[16] = {0};
	m_AxialElements[0] = 1;			//1, 0, 0, 0,
	m_AxialElements[5] = 1;			//0, 1, 0, 0,
	m_AxialElements[10] = 1;		//0, 0, 1, 0,
	m_AxialElements[15] = 1;		//0, 0, 0, 1

	m_SagittalElements[16] = {0};
	m_SagittalElements[2] = -1;		//0, 0, -1, 0,
	m_SagittalElements[4] = -1;		//-1, 0, 0, 0,
	m_SagittalElements[9] = 1;		//0, 1, 0, 0,
	m_SagittalElements[15] = 1;		//0, 0, 0, 1

	m_CoronalElements[16] = {0};
	m_CoronalElements[0] = 1;		//1, 0, 0, 0,
	m_CoronalElements[6] = -1;		//0, 0, -1, 0,
	m_CoronalElements[9] = 1;		//0, 1, 0, 0,
	m_CoronalElements[15] = 1;		//0, 0, 0, 1

	// [0] = window, [1] = level
	m_WinLvCTAbdomen[0] = 350.;
	m_WinLvCTAbdomen[1] = 40.;
	m_WinLvCTAir[0] = 1000.;
	m_WinLvCTAir[1] = -426.;
	m_WinLvCTBone[0] = 1000.;
	m_WinLvCTBone[1] = 400.;
	m_WinLvCTBrain[0] = 100.;
	m_WinLvCTBrain[1] = 50.;
	m_WinLvCTLung[0] = 1400.;
	m_WinLvCTLung[1] = -500;
	m_WinLvCTSoftTissue[0] = 500.;
	m_WinLvCTSoftTissue[1] = 100.;
	m_WinLvMRI[0] = 500.;
	m_WinLvMRI[1] = -774.;
}

ImageKit::~ImageKit()
{

}

void ImageKit::InitialBasicViews()
{
	CalcImageInfo();

	// 0. Display imageData in 2D sliceViews
	m_AxialMapper->SetInputData(m_ImageData);
	m_AxialMapper->SetOrientationToZ();
	m_AxialMapper->SetSliceNumber(m_ImageExtent[4] + m_ImageDimensions[2] / 2.);

	m_SagittalMapper->SetInputData(m_ImageData);
	m_SagittalMapper->SetOrientationToX();
	m_SagittalMapper->SetSliceNumber(m_ImageExtent[0] + m_ImageDimensions[0] / 2.);

	m_CoronalMapper->SetInputData(m_ImageData);
	m_CoronalMapper->SetOrientationToY();
	m_CoronalMapper->SetSliceNumber(m_ImageExtent[2] + m_ImageDimensions[1] / 2.);

	m_AxialSlice->SetMapper(m_AxialMapper);
	m_SagittalSlice->SetMapper(m_SagittalMapper);
	m_CoronalSlice->SetMapper(m_CoronalMapper);

	m_AxialRen->AddViewProp(m_AxialSlice);
	m_SagittalRen->AddViewProp(m_SagittalSlice);
	m_CoronalRen->AddViewProp(m_CoronalSlice);

	// Set the 2D sagittalView and coronalView
	m_SagittalRen->GetActiveCamera()->SetPosition(-1, 0, 0);
	m_SagittalRen->GetActiveCamera()->SetViewUp(0, 0, -1);
	m_CoronalRen->GetActiveCamera()->SetPosition(0, 1, 0);
	m_CoronalRen->GetActiveCamera()->SetViewUp(0, 0, -1);

	// Remove stereo effect from 2D views
	m_AxialRen->GetActiveCamera()->ParallelProjectionOn();
	m_SagittalRen->GetActiveCamera()->ParallelProjectionOn();
	m_CoronalRen->GetActiveCamera()->ParallelProjectionOn();

	// 1. Display imageData in 3D sliceViews
	m_AxialPlaneWidget->SetInputData(m_ImageData);
	m_AxialPlaneWidget->SetInteractor(m_SpatialRen->GetRenderWindow()->GetInteractor());
	m_AxialPlaneWidget->PlaceWidget(m_ImageBounds);
	m_AxialPlaneWidget->SetPlaneOrientationToZAxes();
	m_AxialPlaneWidget->SetMarginSizeX(0.);
	m_AxialPlaneWidget->SetMarginSizeY(0.);
	m_AxialPlaneWidget->SetSliceIndex(m_ImageExtent[4] + m_ImageDimensions[2] / 2.);
	m_AxialPlaneWidget->On();

	m_SagittalPlaneWidget->SetInputData(m_ImageData);
	m_SagittalPlaneWidget->SetInteractor(m_SpatialRen->GetRenderWindow()->GetInteractor());
	m_SagittalPlaneWidget->PlaceWidget(m_ImageBounds);
	m_SagittalPlaneWidget->SetPlaneOrientationToXAxes();
	m_SagittalPlaneWidget->SetMarginSizeX(0.);
	m_SagittalPlaneWidget->SetMarginSizeY(0.);
	m_SagittalPlaneWidget->SetSliceIndex(m_ImageExtent[0] + m_ImageDimensions[0] / 2.);
	m_SagittalPlaneWidget->On();

	m_CoronalPlaneWidget->SetInputData(m_ImageData);
	m_CoronalPlaneWidget->SetInteractor(m_SpatialRen->GetRenderWindow()->GetInteractor());
	m_CoronalPlaneWidget->PlaceWidget(m_ImageBounds);
	m_CoronalPlaneWidget->SetPlaneOrientationToYAxes();
	m_CoronalPlaneWidget->SetMarginSizeX(0.);
	m_CoronalPlaneWidget->SetMarginSizeY(0.);
	m_CoronalPlaneWidget->SetSliceIndex(m_ImageExtent[2] + m_ImageDimensions[1] / 2.);
	m_CoronalPlaneWidget->On();

	m_VTKQtConnector->Connect(m_AxialPlaneWidget,
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnSpatialPlaneWidget(vtkObject*, unsigned long, void*, void*)));
	m_VTKQtConnector->Connect(m_SagittalPlaneWidget,
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnSpatialPlaneWidget(vtkObject*, unsigned long, void*, void*)));
	m_VTKQtConnector->Connect(m_CoronalPlaneWidget,
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnSpatialPlaneWidget(vtkObject*, unsigned long, void*, void*)));
}

void ImageKit::LoadImageToUI()
{
	// 0. Calculate imageData information
	CalcImageInfo();

	auto axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetTotalLength(m_ImageBounds[1],
						 m_ImageBounds[3],
						 m_ImageBounds[5]);
	//axialRen->AddActor(axes);
	//sagittalRen->AddActor(axes);
	m_SpatialRen->AddActor(axes);

	// 1. Setup UI
	// Setup UI: Display
	ui.WindowSpinBox->setRange(0, (int)m_ImageScalarRange[1] - (int)m_ImageScalarRange[0]);
	ui.WindowSliderLeft->setRange(0, (int)m_ImageScalarRange[1] - (int)m_ImageScalarRange[0]);
	ui.WindowSliderRight->setRange(0, (int)m_ImageScalarRange[1] - (int)m_ImageScalarRange[0]);

	ui.LevelSpinBox->setRange((int)m_ImageScalarRange[0], (int)m_ImageScalarRange[1]);
	ui.LevelSlider->setRange((int)m_ImageScalarRange[0], (int)m_ImageScalarRange[1]);

	connect(ui.WindowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnWinLvToGrayscale(int)));
	connect(ui.LevelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnWinLvToGrayscale(int)));
	connect(ui.GrayscaleMin, SIGNAL(valueChanged(int)), this, SLOT(OnGrayscaleToWinLv(int)));
	connect(ui.GrayscaleMax, SIGNAL(valueChanged(int)), this, SLOT(OnGrayscaleToWinLv(int)));

	connect(ui.Presets, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnWinLvPresets(QString)));

	// Update UI from vtkInteractorStyleImage
	m_VTKQtConnector->Connect(m_AxialRen->GetRenderWindow()->GetInteractor()->GetInteractorStyle(),
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnIntStyleImage(vtkObject*, unsigned long, void*, void*)));
	m_VTKQtConnector->Connect(m_SagittalRen->GetRenderWindow()->GetInteractor()->GetInteractorStyle(),
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnIntStyleImage(vtkObject*, unsigned long, void*, void*)));
	m_VTKQtConnector->Connect(m_CoronalRen->GetRenderWindow()->GetInteractor()->GetInteractorStyle(),
							  vtkCommand::InteractionEvent,
							  this,
							  SLOT(OnIntStyleImage(vtkObject*, unsigned long, void*, void*)));

	// Setup UI: Threshold

	// Setup UI: Volume Information
	ui.Directory->setText(m_Directory);
	ui.PatientName->setText(m_PatientName);

	ui.ImageDimensions_0->setRange((double)m_ImageDimensions[0], (double)m_ImageDimensions[0]);
	ui.ImageDimensions_1->setRange((double)m_ImageDimensions[1], (double)m_ImageDimensions[1]);
	ui.ImageDimensions_2->setRange((double)m_ImageDimensions[2], (double)m_ImageDimensions[2]);

	ui.ImageSpacing_0->setValue((double)m_ImageSpacing[0]);
	ui.ImageSpacing_1->setValue((double)m_ImageSpacing[1]);
	ui.ImageSpacing_2->setValue((double)m_ImageSpacing[2]);

	ui.ImageOrigin_0->setValue((double)m_ImageSpacing[0]);
	ui.ImageOrigin_1->setValue((double)m_ImageSpacing[1]);
	ui.ImageOrigin_2->setValue((double)m_ImageSpacing[2]);

	ui.Presets->setCurrentText("Auto");
}

void ImageKit::SetSlicePosition(double positionZ, double positionX, double positionY)
{
	double scale;
	scale = positionZ / (m_ImageBounds[5] - m_ImageBounds[4]);
	int indexZ = m_ImageExtent[4] + scale*(m_ImageExtent[5] - m_ImageExtent[4]);
	scale = positionZ / (m_ImageBounds[1] - m_ImageBounds[0]);
	int indexX = m_ImageExtent[0] + scale*(m_ImageExtent[1] - m_ImageExtent[4]);
	scale = positionZ / (m_ImageBounds[3] - m_ImageBounds[2]);
	int indexY = m_ImageExtent[2] + scale*(m_ImageExtent[3] - m_ImageExtent[2]);

	SetSliceIndex(indexZ, indexX, indexY);
}

void ImageKit::SetSliceIndex(int indexZ, int indexX, int indexY)
{
	if (indexZ != -1)
	{
		m_AxialMapper->SetSliceNumber(indexZ);
		m_AxialPlaneWidget->SetSliceIndex(indexZ);
	}
	if (indexX != -1)
	{
		m_SagittalMapper->SetSliceNumber(indexX);
		m_SagittalPlaneWidget->SetSliceIndex(indexX);
	}
	if (indexY != -1)
	{
		m_CoronalMapper->SetSliceNumber(indexY);
		m_CoronalPlaneWidget->SetSliceIndex(indexY);
	}
}

void ImageKit::SetWindowLevel(double window, double level)
{
	m_AxialSlice->GetProperty()->SetColorWindow(window);
	m_AxialSlice->GetProperty()->SetColorLevel(level);
	m_SagittalSlice->GetProperty()->SetColorWindow(window);
	m_SagittalSlice->GetProperty()->SetColorLevel(level);
	m_CoronalSlice->GetProperty()->SetColorWindow(window);
	m_CoronalSlice->GetProperty()->SetColorLevel(level);

	m_AxialPlaneWidget->SetWindowLevel(window, level);
	m_SagittalPlaneWidget->SetWindowLevel(window, level);
	m_CoronalPlaneWidget->SetWindowLevel(window, level);
}

void ImageKit::SetUIWindowLevel(double window, double level)
{
	m_bWinLvGrayscaleConnect = false;
	ui.WindowSpinBox->setValue(window);
	m_bWinLvGrayscaleConnect = true;
	ui.LevelSpinBox->setValue(level);
}

void ImageKit::SetupRenderer(vtkRenderer* axialRen,
							 vtkRenderer* sagittalRen,
							 vtkRenderer* coronalRen,
							 vtkRenderer* spatialRen)
{
	m_AxialRen = axialRen;
	m_SagittalRen = sagittalRen;
	m_CoronalRen = coronalRen;
	m_SpatialRen = spatialRen;
}

void ImageKit::ReadDICOMImg(QString Directory)
{
	auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(Directory.toStdString().c_str());
	m_VTKQtConnector->Connect(reader,
							  vtkCommand::ProgressEvent,
							  this,
							  SLOT(OnProgress(vtkObject*, unsigned long, void*, void*)));
	reader->Update();
	m_ImageData->DeepCopy(reader->GetOutput());

	m_Directory = reader->GetDirectoryName();
	m_PatientName = reader->GetPatientName();
}

void ImageKit::ReadXMLImg(QString FileName)
{
	auto reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
	reader->SetFileName(FileName.toStdString().c_str());
	reader->Update();
	m_ImageData->DeepCopy(reader->GetOutput());
}

void ImageKit::CalcImageInfo()
{
	// Calculate the Dimensions, Extent, Spacing, Origin, ScalarRange
	m_ImageData->GetDimensions(m_ImageDimensions);
	m_ImageData->GetExtent(m_ImageExtent);
	m_ImageData->GetSpacing(m_ImageSpacing);
	m_ImageData->GetOrigin(m_ImageOrigin);
	m_ImageData->GetScalarRange(m_ImageScalarRange);

	// Calculate the Center
	m_ImageCenter[0] = m_ImageOrigin[0] + m_ImageSpacing[0] * 0.5 * (m_ImageExtent[0] + m_ImageExtent[1]);
	m_ImageCenter[1] = m_ImageOrigin[1] + m_ImageSpacing[1] * 0.5 * (m_ImageExtent[2] + m_ImageExtent[3]);
	m_ImageCenter[2] = m_ImageOrigin[2] + m_ImageSpacing[2] * 0.5 * (m_ImageExtent[4] + m_ImageExtent[5]);

	// Calculate the min/max X,Y,Z
	m_ImageBounds[0] = m_ImageOrigin[0];
	m_ImageBounds[1] = m_ImageOrigin[0] + m_ImageSpacing[0] * (m_ImageExtent[0] + m_ImageExtent[1]);
	m_ImageBounds[2] = m_ImageOrigin[1];
	m_ImageBounds[3] = m_ImageOrigin[1] + m_ImageSpacing[1] * (m_ImageExtent[2] + m_ImageExtent[3]);
	m_ImageBounds[4] = m_ImageOrigin[2];
	m_ImageBounds[5] = m_ImageOrigin[2] + m_ImageSpacing[2] * (m_ImageExtent[4] + m_ImageExtent[5]);

	// Calculata the auto-adapted image Window/Level
	auto histogram = vtkSmartPointer<vtkImageHistogramStatistics>::New();
	histogram->SetInputData(m_ImageData);
	histogram->Update();
	histogram->GetAutoRange(m_ImageAutoRange);

	m_ImageAutoWinLv[0] = m_ImageAutoRange[1] - m_ImageAutoRange[0];
	m_ImageAutoWinLv[1] = (m_ImageAutoRange[0] + m_ImageAutoRange[1]) / 2.;
}

void ImageKit::Render()
{
	m_AxialRen->GetRenderWindow()->Render();
	m_SagittalRen->GetRenderWindow()->Render();
	m_CoronalRen->GetRenderWindow()->Render();
	m_SpatialRen->GetRenderWindow()->Render();
}

void ImageKit::OnWinLvToGrayscale(int)
{
	if (!m_bWinLvGrayscaleConnect)
	{
		return;
	}
	m_bWinLvGrayscaleConnect = false;

	double min = ui.LevelSpinBox->value() - ui.WindowSpinBox->value() / 2.;
	double max = ui.LevelSpinBox->value() + ui.WindowSpinBox->value() / 2.;

	ui.GrayscaleMin->setValue(min);
	ui.GrayscaleMax->setValue(max);
	ui.Presets->setCurrentText("Manual");

	SetWindowLevel(ui.WindowSpinBox->value(), ui.LevelSpinBox->value());
	Render();

	m_bWinLvGrayscaleConnect = true;
}
void ImageKit::OnGrayscaleToWinLv(int)
{
	if (!m_bWinLvGrayscaleConnect)
	{
		return;
	}
	m_bWinLvGrayscaleConnect = false;

	double window = ui.GrayscaleMax->value() - ui.GrayscaleMin->value();
	double level = (ui.GrayscaleMin->value() + ui.GrayscaleMax->value()) / 2.;

	ui.WindowSpinBox->setValue(window);
	ui.LevelSpinBox->setValue(level);
	ui.Presets->setCurrentText("Manual");

	SetWindowLevel(window, level);
	Render();

	m_bWinLvGrayscaleConnect = true;
}

void ImageKit::OnWinLvPresets(QString presetName)
{
	if (presetName == "Manual")
	{
	}
	if (presetName == "Auto")
	{
		SetUIWindowLevel(m_ImageAutoWinLv[0],m_ImageAutoWinLv[1]);
		ui.Presets->setCurrentText("Auto");
	}
	if (presetName == "CT-Abdomen")
	{
		SetUIWindowLevel(m_WinLvCTAbdomen[0],m_WinLvCTAbdomen[1]);
		ui.Presets->setCurrentText("CT-Abdomen");
	}
	if (presetName == "CT-Air")
	{
		SetUIWindowLevel(m_WinLvCTAir[0],m_WinLvCTAir[1]);
		ui.Presets->setCurrentText("CT-Air");
	}
	if (presetName == "CT-Bone")
	{
		SetUIWindowLevel(m_WinLvCTBone[0],m_WinLvCTBone[1]);
		ui.Presets->setCurrentText("CT-Bone");
	}
	if (presetName == "CT-Brain")
	{
		SetUIWindowLevel(m_WinLvCTBrain[0],m_WinLvCTBrain[1]);
		ui.Presets->setCurrentText("CT-Brain");
	}
	if (presetName == "CT-Lung")
	{
		SetUIWindowLevel(m_WinLvCTLung[0],m_WinLvCTLung[1]);
		ui.Presets->setCurrentText("CT-Lung");
	}
}

void ImageKit::OnProgress(vtkObject* caller, unsigned long eventID, void* clientData, void* callData)
{
	double* progress = reinterpret_cast<double*>(callData);
	if (*progress == 0.)
	{
		ui.ProgressBar->show();
	}

	ui.ProgressBar->setValue((int)(*progress*100));

	if (*progress == 1.)
	{
		ui.ProgressBar->hide();
	}
}

void ImageKit::OnIntStyleImage(vtkObject* caller, unsigned long eventID, void* clientData, void* callData)
{
	auto intStyleImage = vtkInteractorStyleImage::SafeDownCast(caller);
	auto imageProperty = intStyleImage->GetCurrentImageProperty();

	if (imageProperty == NULL)
	{
		return;
	}

	double window = imageProperty->GetColorWindow();
	double level = imageProperty->GetColorLevel();

	SetUIWindowLevel(window,level);
}

void ImageKit::OnSpatialPlaneWidget(vtkObject* caller, unsigned long eventID, void* clientData, void* callData)
{
	auto imagePlaneWidget = vtkImagePlaneWidget::SafeDownCast(caller);

	// Update sliceIndex
	int slice = imagePlaneWidget->GetSliceIndex();
	int orientation = imagePlaneWidget->GetPlaneOrientation();

	if (orientation == 2) // normal Z-axis, Axial
	{
		m_AxialMapper->SetSliceNumber(slice);
		emit ChangeSliceIndex(slice, -1, -1);
	}
	if (orientation == 0) // normal X-axis, Sagittal
	{
		m_SagittalMapper->SetSliceNumber(slice);
		emit ChangeSliceIndex(-1, slice, -1);
	}
	if (orientation == 1) // normal Y-axis, Coronal
	{
		m_CoronalMapper->SetSliceNumber(slice);
		emit ChangeSliceIndex(-1, -1, slice);
	}

	Render();
}