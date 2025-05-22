#include "Interop.h"
#include "vector"
#include "string.h"
#include "windows.h"
#include "map"
#include "unordered_map"
#include "string.h"
#include "Model/Reader/Tokenized/ModelToken.h"
#include "Model/Reader/ModelReader.h"
#include "Model/Creator/ModelCreator.h"
#include "Render/Render/ModelRenderer.h"
#include "Render/Scene/Object/Cube.h";
#include "Model/Creator/ModelParameters.h"
#include "Render/Paint/Image/WriteonlyImage.h"
#include "Render/Paint/Image/Bitmap_BGRA32.h"
#include <Render/Scene/Camera/ProjectionCamera.h>
#include <Render/Scene/Camera/PerspectiveCamera.h>
#include "../Model.Sampler/SampleTemplate/SampleParameters.h"
#include "../Model.Sampler/SampleTemplate/SampleTemplate.h"
#include <execution>
#include <Blockstate/Reader/BlockstateReader.h>
#include <Blockstate/SizeCalculator.h>

std::map<std::string, ModelToken*>* m_tokens;
std::unordered_map<std::string, Model*>* m_models = new std::unordered_map<std::string, Model*>();

ModelReader* m_reader;
BlockstateReader* m_blockStateReader;
ModelCreator* m_creator;
ModelRenderer* m_renderer;

ProjectionCamera* m_projCamera = new ProjectionCamera();
PerspectiveCamera* m_perspectivejCamera = new PerspectiveCamera();

SampleTemplate m_template;
Model* m_lastModel;

Bitmap_BGRA32* m_bitmap;

int initialize() {
    m_reader = new ModelReader();
    m_renderer = new ModelRenderer();

    Environment e = Environment();
    e.EnvironmentShading.ShadingEnabled = false;
    e.SkyColor = Rgba(0, 0);

    ProjectionCamera* camera = new ProjectionCamera();
    camera->Location = Vec3f(8 - 16 * 4, 16 * 3.5, 8 + 16 * 4);
    camera->PlaneWidth = 16 * 2;
    camera->LookAt(8);

    e.EnvironmentCamera = camera;

    SampleParameters sample = SampleParameters();
    sample.SampleEnvironment = e;
    sample.CuboidVolume = BoundingBox(0, 16);
    sample.Width = 128;
    sample.Height = 128;

    m_template = SampleTemplate();
    m_template.Initialize(sample);

	return 0;
}

Model* createModel(const char* name) {
    ModelToken* token = (*m_tokens)[name];
    ModelParameters parameters = ModelParameters(0, 0, false, token->Name.find("glass") != std::string::npos || token->Name == "water" || token->Name == "ice");

    Model* model;
    if (m_models->find(token->Name) == m_models->end()) {
        model = m_creator->CreateModel(*token, parameters);
        (*m_models)[token->Name] = model;
    }
    else {
        model = (*m_models)[token->Name];
    }

    return model;
}

static std::vector<int> m_prevRange;

void loadDirectory(const char* directory) {
    m_reader = new ModelReader();
    m_tokens = m_reader->Read(std::string(directory) + "\\models");
    m_creator = new ModelCreator(std::string(directory) + "\\textures");

    m_blockStateReader = new BlockstateReader();
    std::map<std::string, BlockstateToken*>* blockStates = m_blockStateReader->Read(std::string(directory) + "\\blockstates");

    SizeCalculator calculator = SizeCalculator();

    int total = 0;
    for (const auto& pair : *blockStates) {
        total += calculator.Calculate(*pair.second);
    }

    int angles = 9 * 0;

    std::cout << total << " TOTAL\n";

    Model** models = new Model*[m_tokens->size()];
    int index = 0;
    for (const auto& pair : *m_tokens) {
        models[index] = createModel(pair.first.c_str());
        ++index;
    }

    m_prevRange = std::vector<int>(m_tokens->size());
    std::iota(m_prevRange.begin(), m_prevRange.end(), 0);
    
    int t = (int)(total / (float)m_tokens->size() * angles);

    long long e = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::for_each(std::execution::par, m_prevRange.begin(), m_prevRange.end(),
    [&, t](int& y) {
            for (int i = 0; i < t; i++) {
                SampleResult result = m_template.Sample(models[y]);
                e += result.Volume.InvisibleFaceRays;
            }
    });

    std::cout << "Frames rendered: " << (t * m_tokens->size()) << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
}

void provideModelNames(const char**& arr, int& size) {
	std::vector<const char*> converted = std::vector<const char*>();
    for (auto& [key, value] : *m_tokens) {
        converted.push_back(value->Name.c_str());
    }

	arr = converted.data();
	size = static_cast<int>(converted.size());
}

Bitmap_BGRA32* GetBitmap(int width, int height) {
    if (m_bitmap == NULL) {
        m_bitmap = new Bitmap_BGRA32(width, height);
    }

    if (m_bitmap->GetWidth() != width || m_bitmap->GetHeight() != height) {
        delete m_bitmap;
        m_bitmap = new Bitmap_BGRA32(width, height);
    }

    return m_bitmap;
}

Camera* CreateCamera(const CameraInterop& args) {
    if (args.CameraType == CameraTypeInterop::Projection) {
        m_projCamera->Location = args.Location.ToVec3f();
        m_projCamera->PlaneRotation = Rotation::FromRadians(0);
        m_projCamera->PlaneWidth = args.PlaneWidth;
        m_projCamera->SetDirection(args.Direction.ToVec3f());

        return m_projCamera;
    }
    else {
        m_perspectivejCamera->Location = args.Location.ToVec3f();
        m_perspectivejCamera->PlaneRotation = Rotation::FromRadians(0);
        m_perspectivejCamera->FocalLength = args.FocalLenght;
        m_perspectivejCamera->SetDirection(args.Direction.ToVec3f());

        return m_perspectivejCamera;
    }
}

Environment CreateEnvironment(const EnvironmentInterop& args) {
    Shading shading = Shading();
    shading.ShadingEnabled = args.Shading.ShadingEnabled;
    shading.AmbientLight = args.Shading.AmbientLight.ToVec3f();
    shading.DirectionalLightDirection = args.Shading.DirectionalLightDirection.ToVec3f();
    shading.DirectionalLight = args.Shading.DirectionalLight.ToVec3f();

    Environment output = Environment();
    output.EnvironmentCamera = CreateCamera(args.Camera);
    output.EnvironmentShading = shading;
    output.SkyColor = Rgba(args.SkyColor.Rgb.ToVec3f(), args.SkyColor.A);

    return output;
}

std::string ToPercentage(float value) {
    if (std::isnan(value)) value = 0;

    float percentage = value * 100.0f;

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << percentage << "%";

    return stream.str();
}

uint8_t* requestModelRender(ModelRenderRequestArgs args) {
    Model* model = createModel(args.Name);

    Bitmap_BGRA32* image = GetBitmap(args.Width, args.Height);
    Environment environment = CreateEnvironment(args.Environment);

    m_creator->TickTextureProvider->UpateTickBasedOnCurrentTime();

    m_renderer->Render(environment, model, args.Width, args.Height, image);

    if (model != m_lastModel) {
        m_lastModel = model;
        SampleResult result = m_template.Sample(model);

        //system("cls");

        float sum = result.Volume.VisibleFaceRays + result.Volume.VisibleFaceTransparentRays + result.Volume.InvisibleFaceRays;

        float volume = (result.Volume.VisibleFaceRays / (float)result.Volume.TotalCuboidArea);
        std::cout << "Average color (R, G, B, A, Tint):\t\t\t[" << (int)(result.Color.AverageColorOfVisibleFace.Rgb.X * 255);
        std::cout << ", " << (int)(result.Color.AverageColorOfVisibleFace.Rgb.Y * 255);
        std::cout << ", " << (int)(result.Color.AverageColorOfVisibleFace.Rgb.Z * 255);
        std::cout << ", " << (int)(result.Color.AverageColorOfVisibleFace.A * 255);
        std::cout << ", " << result.Color.AverageColorOfVisibleFace.Tint << "]\n";
        std::cout << "Visible absolute volume:\t\t\t\t" << ToPercentage(volume) << "\n";
        std::cout << "Visible volume (% of model cuboid):\t\t\t" << ToPercentage(result.Volume.VisibleFaceRays / sum) << "\n";
        std::cout << "Visible transparent face volume (% of model cuboid):\t" << ToPercentage(result.Volume.VisibleFaceTransparentRays / sum) << "\n";
        std::cout << "Invisible face volume (% of model cuboid):\t\t" << ToPercentage(result.Volume.InvisibleFaceRays / sum) << "\n";
        std::cout << "End absolute opacity:\t\t\t\t\t" << ToPercentage(volume * result.Color.AverageColorOfVisibleFace.A) << "\n";
    }

    return image->GetBuffer();
}

ModelInfo requestModelInfo(const char* name) {
    Model* model = createModel(name);

    ModelInfo output = ModelInfo();

    output.From.X = model->Box.GetMin().X;
    output.From.Y = model->Box.GetMin().Y;
    output.From.Z = model->Box.GetMin().Z;

    output.To.X = model->Box.GetMax().X;
    output.To.Y = model->Box.GetMax().Y;
    output.To.Z = model->Box.GetMax().Z;

    return output;
}

void dispose() {
	
}