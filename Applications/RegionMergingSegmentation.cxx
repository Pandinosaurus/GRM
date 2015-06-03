#include <iostream>
#include <otbImage.h>
#include <otbVectorImage.h>
#include <otbImageFileReader.h>
#include <otbImageFileWriter.h>
#include "lsrmBaatzSegmenter.h"

int main(int argc, char *argv[])
{
	if(argc != 7)
	{
		std::cerr << "Usage: ./" << argv[0] << "\n"
				  << "\t[input image path] : (.jpg, .png, .tif)\n"
				  << "\t[output clustered image] : (.jpg, .png, .tif)\n"
				  << "\t[output label image] : (.tif)\n"
				  << "\t[spectral weight] : range from 0 to 1\n"
				  << "\t[shape weight] : range from 0 to 1\n"
				  << "\t[scale threshold] : unlimited positive value"
				  << std::endl;
		return 1;
	}

	lsrm::BaatzParam params;
	const char * inFileName = argv[1];
	const char * clusterFileName = argv[2];
	const char * labelFileName = argv[3];
	params.m_SpectralWeight = atof(argv[4]);
	params.m_ShapeWeight = atof(argv[5]);
	float sqrt_scale = atof(argv[6]);
	const float scale = sqrt_scale * sqrt_scale;

	typedef float PixelType;
	typedef unsigned long int LabelPixelType;
	typedef unsigned char ClusterPixelType;
	typedef otb::VectorImage<PixelType, 2> InputImageType;
	typedef otb::Image<LabelPixelType, 2> LabelImageType;
	typedef otb::VectorImage<ClusterPixelType, 2> ClusterImageType;
	typedef otb::ImageFileReader<InputImageType> InputImageReaderType;
	typedef otb::ImageFileWriter<LabelImageType> LabelImageWriterType;
	typedef otb::ImageFileWriter<ClusterImageType> ClusterImageWriterType;
	typedef lsrm::BaatzSegmenter<InputImageType> SegmenterType;

	auto imgReader = InputImageReaderType::New();
	imgReader->SetFileName(inFileName);
	imgReader->Update();

	
	SegmenterType segmenter;
	segmenter.SetParam(params);
	segmenter.SetThreshold(scale);
	segmenter.SetInput(imgReader->GetOutput());
	segmenter.Update();

	auto labelWriter = LabelImageWriterType::New();
	labelWriter->SetFileName(labelFileName);
	labelWriter->SetInput(segmenter.GetLabeledClusteredOutput());
	labelWriter->Update();

	auto clusterWriter = ClusterImageWriterType::New();
	clusterWriter->SetFileName(clusterFileName);
	clusterWriter->SetInput(segmenter.GetClusteredImageOutput());
	clusterWriter->Update();
	
    return 0;
}








