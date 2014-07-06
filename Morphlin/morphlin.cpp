#include "Morphlin.h"

Morphlin::Morphlin()
	: QMainWindow()
{
	ui.setupUi(this);
	ui.ToolkitDockWidget->hide();

	m_ImageKit = new ImageKit();

	m_SliceView = new SliceView();
	m_SliceView->SetWidget(ui.AxialViewWidget,
						   ui.SagittalViewWidget,
						   ui.CoronalViewWidget,
						   ui.SpatialViewWidget);

	connect(ui.LoadDICOMAction, SIGNAL(triggered()), this, SLOT(OnLoadDICOM()));
	connect(ui.ImageKitAction, SIGNAL(triggered()), this, SLOT(OnImageKit()));

	connect(m_ImageKit, SIGNAL(ChangeSliceIndex(int, int, int)), this, SLOT(OnChangeSliceIndex(int, int, int)));
}

Morphlin::~Morphlin()
{
	delete m_ImageKit;
	delete m_SliceView;
}

void Morphlin::OnLoadDICOM()
{
	QString Directory = "E:/DICOMData/bingu";
	m_ImageKit->ReadDICOMImg(Directory);

	if (!m_ImageKit->m_bInitialized)
	{
		m_ImageKit->SetupRenderer(m_SliceView->m_AxialRen,
								m_SliceView->m_SagittalRen,
								m_SliceView->m_CoronalRen,
								m_SliceView->m_SpatialRen);

		m_ImageKit->InitialBasicViews();
	}
	m_ImageKit->m_bInitialized = true;

	m_ImageKit->LoadImageToUI();

	SetupSliceViewBar();
	OnImageKit();

	m_SliceView->ResetCamera();
	m_SliceView->Render();
}

void Morphlin::OnImageKit()
{
	ui.ToolkitDockWidget->setWidget(m_ImageKit);
	ui.ToolkitDockWidget->setWindowTitle("ImageKit");
	ui.ToolkitDockWidget->setFloating(false);
	ui.ToolkitDockWidget->show();
}

void Morphlin::SetupSliceViewBar()
{
	int extent[6];
	double spacing[3];
	m_ImageKit->m_ImageData->GetExtent(extent);
	m_ImageKit->m_ImageData->GetSpacing(spacing);

	ui.AxiScrollBar->setMinimum(extent[4]);
	ui.AxiScrollBar->setMaximum(extent[5]);
	ui.SagScrollBar->setMinimum(extent[0]);
	ui.SagScrollBar->setMaximum(extent[1]);
	ui.CorScrollBar->setMinimum(extent[2]);
	ui.CorScrollBar->setMaximum(extent[3]);
	ui.AxiScrollBar->setSingleStep(1);
	ui.SagScrollBar->setSingleStep(1);
	ui.CorScrollBar->setSingleStep(1);

	ui.AxiSpinBox->setMinimum(m_ImageKit->m_ImageBounds[4]);
	ui.AxiSpinBox->setMaximum(m_ImageKit->m_ImageBounds[5]);
	ui.SagSpinBox->setMinimum(m_ImageKit->m_ImageBounds[0]);
	ui.SagSpinBox->setMaximum(m_ImageKit->m_ImageBounds[1]);
	ui.CorSpinBox->setMinimum(m_ImageKit->m_ImageBounds[2]);
	ui.CorSpinBox->setMaximum(m_ImageKit->m_ImageBounds[3]);
	ui.AxiSpinBox->setSingleStep(1);
	ui.SagSpinBox->setSingleStep(1);
	ui.CorSpinBox->setSingleStep(1);

	ui.AxiScrollBar->setValue((extent[4] + extent[5]) / 2.);
	ui.SagScrollBar->setValue((extent[0] + extent[1]) / 2.);
	ui.CorScrollBar->setValue((extent[2] + extent[3]) / 2.);
	ui.AxiSpinBox->setValue((m_ImageKit->m_ImageBounds[4] + m_ImageKit->m_ImageBounds[5]) / 2.);
	ui.SagSpinBox->setValue((m_ImageKit->m_ImageBounds[0] + m_ImageKit->m_ImageBounds[1]) / 2.);
	ui.CorSpinBox->setValue((m_ImageKit->m_ImageBounds[2] + m_ImageKit->m_ImageBounds[3]) / 2.);

	connect(ui.AxiScrollBar, SIGNAL(valueChanged(int)), this, SLOT(OnScrollBarToSpinBox(int)));
	connect(ui.SagScrollBar, SIGNAL(valueChanged(int)), this, SLOT(OnScrollBarToSpinBox(int)));
	connect(ui.CorScrollBar, SIGNAL(valueChanged(int)), this, SLOT(OnScrollBarToSpinBox(int)));
	connect(ui.AxiSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnSpinBoxToScrollBar(double)));
	connect(ui.SagSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnSpinBoxToScrollBar(double)));
	connect(ui.CorSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnSpinBoxToScrollBar(double)));
}

void Morphlin::OnScrollBarToSpinBox(int)
{
	if (!m_bScrollBarSpinBoxConnect)
	{
		return;
	}
	m_bScrollBarSpinBoxConnect = false;

	double scale;
	scale = ui.AxiScrollBar->value() / (double)(ui.AxiScrollBar->maximum() - ui.AxiScrollBar->minimum());
	ui.AxiSpinBox->setValue(ui.AxiSpinBox->minimum() + scale*(ui.AxiSpinBox->maximum() - ui.AxiSpinBox->minimum()));
	scale = ui.SagScrollBar->value() / (double)(ui.SagScrollBar->maximum() - ui.SagScrollBar->minimum());
	ui.SagSpinBox->setValue(ui.SagSpinBox->minimum() + scale*(ui.SagSpinBox->maximum() - ui.SagSpinBox->minimum()));
	scale = ui.CorScrollBar->value() / (double)(ui.CorScrollBar->maximum() - ui.CorScrollBar->minimum());
	ui.CorSpinBox->setValue(ui.CorSpinBox->minimum() + scale*(ui.CorSpinBox->maximum() - ui.CorSpinBox->minimum()));

	m_ImageKit->SetSliceIndex(ui.AxiScrollBar->value(),
							  ui.SagScrollBar->value(),
							  ui.CorScrollBar->value());

	m_SliceView->Render();
	m_bScrollBarSpinBoxConnect = true;
}

void Morphlin::OnSpinBoxToScrollBar(double)
{
	if (!m_bScrollBarSpinBoxConnect)
	{
		return;
	}
	m_bScrollBarSpinBoxConnect = false;

	double scale;
	scale = ui.AxiSpinBox->value() / (ui.AxiSpinBox->maximum() - ui.AxiSpinBox->minimum());
	ui.AxiScrollBar->setValue(ui.AxiScrollBar->minimum() + scale*(ui.AxiScrollBar->maximum() - ui.AxiScrollBar->minimum()));
	scale = ui.SagSpinBox->value() / (ui.SagSpinBox->maximum() - ui.SagSpinBox->minimum());
	ui.SagScrollBar->setValue((int)(ui.SagScrollBar->minimum() + scale*(ui.SagScrollBar->maximum() - ui.SagScrollBar->minimum())));
	scale = ui.CorSpinBox->value() / (ui.CorSpinBox->maximum() - ui.CorSpinBox->minimum());
	ui.CorScrollBar->setValue((int)(ui.CorScrollBar->minimum() + scale*(ui.CorScrollBar->maximum() - ui.CorScrollBar->minimum())));

	m_ImageKit->SetSliceIndex(ui.AxiScrollBar->value(),
							  ui.SagScrollBar->value(),
							  ui.CorScrollBar->value());

	m_SliceView->Render();
	m_bScrollBarSpinBoxConnect = true;
}

void Morphlin::OnChangeSliceIndex(int axi, int sag, int cor)
{
	if (axi != -1)
	{
		ui.AxiScrollBar->setValue(axi);
	}
	if (sag != -1)
	{
		ui.SagScrollBar->setValue(sag);
	}
	if (cor != -1)
	{
		ui.CorScrollBar->setValue(cor);
	}
}