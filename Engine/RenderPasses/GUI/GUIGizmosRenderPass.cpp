#include "GUIGizmosRenderPass.h"

#include "Systems/GUI/Gizmo/Gizmo.h"

#include "Managers/ShaderManager.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

GUIGizmosRenderPass::GUIGizmosRenderPass () :
	_shaderName ("EDITOR_GIZMOS")
{

}

void GUIGizmosRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for editor gizmos
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/GUI/guiGizmosVertex.glsl",
		"Assets/Shaders/GUI/guiGizmosFragment.glsl");
}

RenderVolumeCollection* GUIGizmosRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start gizmo draw
	*/

	StartGizmoDraw (rvc);

	/*
	 * Draw gizmos
	*/

	DrawGizmos (camera);

	return rvc;
}

void GUIGizmosRenderPass::StartGizmoDraw (RenderVolumeCollection* rvc)
{
	/*
	 * Bind framebuffer for writing
	*/

	FrameBuffer2DVolume* framebuffer = (FrameBuffer2DVolume*) rvc->GetRenderVolume ("LightAccumulationVolume");
	framebuffer->BindForWriting ();
}

void GUIGizmosRenderPass::DrawGizmos (const Camera* camera)
{
	DrawLines (camera);
}

void GUIGizmosRenderPass::DrawLines (const Camera* camera)
{
	const std::vector<GizmoLine>& lines = Gizmo::GetLines ();

	if (lines.size () == 0) {
		return;
	}

	std::size_t dataSize = 0;
	std::size_t depthDataSize = 0;

	for (auto line : lines) {
		if (line.depthTest == true) {
			depthDataSize ++;
		}

		if (line.depthTest == false) {
			dataSize ++;
		}
	}

	std::vector<float> renderData (dataSize * 3 * 2 * 2);
	std::vector<float> depthRenderData (depthDataSize * 3 * 2 * 2);
	std::vector<unsigned int> indices (dataSize * 2);
	std::vector<unsigned int> depthIndices (depthDataSize * 2);

	std::size_t linesCount = 0;
	std::size_t depthLinesCount = 0;

	for (std::size_t lineIndex = 0; lineIndex < lines.size (); lineIndex ++) {

		if (lines [lineIndex].depthTest == true) {
			std::size_t stride = depthLinesCount * 3 * 2 * 2;

			AddLine (depthRenderData, stride, lines [lineIndex]);
			
			depthIndices [depthLinesCount * 2] = depthLinesCount * 2;
			depthIndices [depthLinesCount * 2 + 1] = depthLinesCount * 2 + 1;

			++ depthLinesCount;
		}

		if (lines [lineIndex].depthTest == false) {
			std::size_t stride = linesCount * 3 * 2 * 2;

			AddLine (renderData, stride, lines [lineIndex]);

			indices [linesCount * 2] = linesCount * 2;
			indices [linesCount * 2 + 1] = linesCount * 2 + 1;

			++ linesCount;
		}
	}

	DrawData (renderData, indices, camera);
	DrawData (depthRenderData, depthIndices, camera, true);
}

void GUIGizmosRenderPass::AddLine (std::vector<float>& renderData, std::size_t stride, const GizmoLine& line)
{
	renderData [stride] = line.first.x;
	renderData [stride + 1] = line.first.y;
	renderData [stride + 2] = line.first.z;
	renderData [stride + 3] = line.color.x;
	renderData [stride + 4] = line.color.y;
	renderData [stride + 5] = line.color.z;

	renderData [stride + 6] = line.last.x;
	renderData [stride + 7] = line.last.y;
	renderData [stride + 8] = line.last.z;
	renderData [stride + 9] = line.color.x;
	renderData [stride + 10] = line.color.y;
	renderData [stride + 11] = line.color.z;
}

void GUIGizmosRenderPass::DrawData (const std::vector<float>& renderData, const std::vector<unsigned int>& indices, const Camera* camera, bool depthTest)
{
	unsigned int VAO;
	GL::GenVertexArrays(1 , &VAO);
	GL::BindVertexArray(VAO);

	unsigned int VBO;
	GL::GenBuffers (1, &VBO);
	GL::BindBuffer (GL_ARRAY_BUFFER, VBO);
	GL::BufferData (GL_ARRAY_BUFFER, sizeof(float) * renderData.size (), renderData.data (), GL_STATIC_DRAW);

	unsigned int IBO;
	GL::GenBuffers (1, &IBO);
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size (), indices.data (), GL_STATIC_DRAW);

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));

	Pipeline::SetObjectTransform (Transform::Default ());
	Pipeline::SendCamera (camera);

	Pipeline::UpdateMatrices (nullptr);

	if (depthTest == true) {
		GL::Enable (GL_DEPTH_TEST);
	}

	if (depthTest == false) {
		GL::Disable (GL_DEPTH_TEST);
	}

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GL::EnableVertexAttribArray(0);
	GL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 6, (void*) 0);
	GL::EnableVertexAttribArray(1);
	GL::VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 6, (void*)(sizeof(float) * 3));

	GL::DrawElements(GL_LINES, indices.size (), GL_UNSIGNED_INT, 0);

	Pipeline::UnlockShader ();

	GL::DeleteBuffers (1, &VBO);
	GL::DeleteVertexArrays (1, &VAO);
}

void GUIGizmosRenderPass::Clear ()
{
	/*
	 * Do nothing
	*/
}
