#include "SSAOSamplesVolume.h"

#include <cstring>

#include "Core/Random/Random.h"

#include "Wrappers/OpenGL/GL.h"

SSAOSamplesVolume::SSAOSamplesVolume () :
	_samplesUBO (0)
{

}

bool SSAOSamplesVolume::Init (std::size_t samplesCount)
{
	/*
	 * Initialize samples
	*/

	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec3 sample (
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			Random::Instance ()->RangeF (0.0f, 1.0f)
		);

		/*
		 * Normalize and multiply with random value in [0, 1) to
		 * keep the value inside the hemisphere
		*/

		sample = glm::normalize (sample);
		sample *= Random::Instance ()->RangeF (0.0f, 1.0f);

		// float scale = (float) index / samplesCount;
		// scale = 0.1f + 0.9f * scale * scale;
		// sample *= scale;

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
		_samples.samples [index * 4 + 2] = sample.z;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	return true;
}

void SSAOSamplesVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void SSAOSamplesVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> SSAOSamplesVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssaoSamples;

	ssaoSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	ssaoSamples.name = "ssaoSamples";

	ssaoSamples.value.x = _samplesUBO;

	attributes.push_back (ssaoSamples);

	return attributes;
}

std::size_t SSAOSamplesVolume::GetSamplesCount () const
{
	return _samples.samplesCount;
}

void SSAOSamplesVolume::Clear ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}