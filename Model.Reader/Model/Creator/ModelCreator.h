#pragma once
#include "string.h"
#include "../../../Model.Render/Render/Scene/Object/Model.h"
#include "ModelParameters.h"
#include "../Reader/Tokenized/ModelToken.h"
#include "Image/ImageProvider.h"
#include "Image/Animation/AnimationProvider.h"
#include "Image/TickProvider.h"
#include "Surface/UvReadonlyImage.h"
#include "Surface/SolidReadonlyImage.h"

class ModelCreator
{
public:
	ImageProvider* ImageProv;
    AnimationProvider* AnimationProv;
    TickProvider* TickTextureProvider;
	std::string FullPath;

	ReadonlyImage* Empty = new SolidReadonlyImage(Rgba(Vec3f(214, 127, 255) / 255, 1));

	ModelCreator(std::string path) {
        ImageProv = new ImageProvider(path);
        AnimationProv = new AnimationProvider(path);
        FullPath = path;
        TickTextureProvider = new TickProvider();

        LockedBitmap* missingTextureBitmap = ImageProv->Provide("missing_texture", UriKind::Relative);
        if (missingTextureBitmap != NULL){
            UvImageParameters parameters = UvImageParameters();
            parameters.TextureUv = Uv(Vec2f(0, 0), Vec2f(16, 16));

            Empty = new UvReadonlyImage(missingTextureBitmap, parameters, new Animation());
        }
	}

    Model* CreateModel(const ModelToken& modelToken, const ModelParameters& modelParameters) const {
        Model* output = new Model();
        output->XRotation = Rotation::FromDegrees(modelParameters.DegreesX);
        output->YRotation = Rotation::FromDegrees(modelParameters.DegreesY);
        output->AllowsSemiTransparency = modelParameters.AllowSemiTransparency;

        for (int i = 0; i < modelToken.Elements.size(); i++) {
            Cube cube = CreateElement(modelToken.Elements[i], modelToken, modelParameters);
            output->Elements.push_back(cube);
        }

        output->UpdateBoundingBox();

        return output;
    };

    Cube CreateElement(const ElementToken& element, const ModelToken& model, const ModelParameters& modelParameters) const {
        Vec3f from = element.From, to = element.To;
        AdjustScale(element, from, to);

        Cube output = Cube(from, to);
        
        if (element.Rotation != NULL) output.Rotation = element.Rotation->Rotation;
        output.Shade = element.Shade;

        Face** faces = (Face**) _malloca(sizeof(Face**) * 6);
        FaceToken** tokens = (FaceToken**)_malloca(sizeof(FaceToken**) * 6);
        int* uvs = (int*) _malloca(sizeof(int*) * 6);
        Axis* axis = (Axis*) _malloca(sizeof(Axis*) * 6);

        faces[0] = &output.Down;
        tokens[0] = element.Down;
        uvs[0] = AdjustSizeTop(modelParameters);
        axis[0] = Axis::Y;

        faces[1] = &output.Up;
        tokens[1] = element.Up;
        uvs[1] = AdjustSizeY(modelParameters);
        axis[1] = Axis::Y;

        faces[2] = &output.North;
        tokens[2] = element.North;
        uvs[2] = AdjustSideZNorth(modelParameters);
        axis[2] = Axis::Z;

        faces[3] = &output.South;
        tokens[3] = element.South;
        uvs[3] = AdjustSideZ(modelParameters);
        axis[3] = Axis::Z;

        faces[4] = &output.West;
        tokens[4] = element.West;
        uvs[4] = -AdjustSideX(modelParameters);
        axis[4] = Axis::X;

        faces[5] = &output.East;
        tokens[5] = element.East;
        uvs[5] = AdjustSideX(modelParameters);
        axis[5] = Axis::X;

        for (int i = 0; i < 6; i++) {
            if (tokens[i] == NULL) faces[i]->Visible = false;
            else
            {
                std::string texturePath;
                GetTexturePath(*tokens[i]->Texture, model, texturePath);

                Uv uv;
                if (tokens[i]->Uv == NULL) uv = GenerateUv(element, axis[i]);
                else uv = *tokens[i]->Uv;

                int rotation = tokens[i]->Rotation;
                if (!modelParameters.UvLock) uvs[i] = 0;

                UvImageParameters parameters = UvImageParameters();
                parameters.TextureUv = uv;
                parameters.TextureRotation = rotation;
                parameters.UvRotation = Mod(uvs[i], 360);
                parameters.AllowSemiTransparency = modelParameters.AllowSemiTransparency;
                parameters.TintIndex = tokens[i]->TintIndex;

                faces[i]->Surface = ProvideImage(texturePath, parameters);
            }
        }

        return output;
    }

    void AdjustScale(const ElementToken& element, Vec3f& from, Vec3f& to) const {
        if (element.Rotation != NULL && element.Rotation->Angle > 0 && element.Rotation->Rescale)
        {
            switch (element.Rotation->Rotation.RotationAxis)
            {
            case Axis::X:
            {
                float angle = (float)cosf(45 * PI / 180);

                float sizeY = to.Y - from.Y;
                float addedSizeY = sizeY / angle - sizeY;

                float sizeZ = to.Z - from.Z;
                float addedSizeZ = sizeZ / angle - sizeZ;

                from = Vec3f(from.X, from.Y - addedSizeY / 2, from.Z - addedSizeZ / 2);
                to = Vec3f(to.X, to.Y + addedSizeY / 2, to.Z + addedSizeZ / 2);
            }
            break;
            case Axis::Y:
            {
                float angle = (float)cosf(45 * PI / 180);

                float sizeX = to.X - from.X;
                float addedSizeX = sizeX / angle - sizeX;

                float sizeZ = to.Z - from.Z;
                float addedSizeZ = sizeZ / angle - sizeZ;

                from = Vec3f(from.X - addedSizeX / 2, from.Y, from.Z - addedSizeZ / 2);
                to = Vec3f(to.X + addedSizeX / 2, to.Y, to.Z + addedSizeZ / 2);
            }
            break;
            default:
            {
                float angle = (float)cosf(45 * PI / 180);

                float sizeX = to.X - from.X;
                float addedSizeX = sizeX / angle - sizeX;

                float sizeY = to.Y - from.Y;
                float addedSizeY = sizeY / angle - sizeY;

                from = Vec3f(from.X - addedSizeX / 2, from.Y - addedSizeY / 2, from.Z);
                to = Vec3f(to.X + addedSizeX / 2, to.Y + addedSizeY / 2, to.Z);
            }
            break;
            }
        }
    }

    int AdjustSideZ(const ModelParameters& parameters) const {
        if (parameters.DegreesX == 180) return 180;
        if (parameters.DegreesX == 0) return 0;

        switch (parameters.DegreesY)
        {
        case 0:
            return 0;
        case 90:
            if (parameters.DegreesX == 90) return -90;
            if (parameters.DegreesX == 270) return 90;
            return 0;
        case 180:
            return 180;
        default:
            if (parameters.DegreesX == 90) return 90;
            return -90;
        }
    }

    int AdjustSideZNorth(const ModelParameters& parameters) const {
        if (parameters.DegreesX == 180) return 180;
        if (parameters.DegreesX == 0) return 0;

        switch (parameters.DegreesY)
        {
        case 0:
            if (parameters.DegreesX == 90) return 180;
            if (parameters.DegreesX == 270) return 180;
            return 0;
        case 90:
            if (parameters.DegreesX == 90) return -90;
            if (parameters.DegreesX == 270) return 90;
            return 0;
        case 180:
            if (parameters.DegreesX == 90) return 0;
            if (parameters.DegreesX == 270) return 0;
            return 180;
        default:
            if (parameters.DegreesX == 90) return 90;
            return -90;
        }
    }

    int AdjustSideX(const ModelParameters& parameters) const {
        if (parameters.DegreesX == 90) return -90;
        if (parameters.DegreesX == 180) return 180;
        if (parameters.DegreesX == 270) return 90;

        return 0;
    }

    int AdjustSizeY(const ModelParameters& parameters) const {
        switch (parameters.DegreesY)
        {
        case 0:
            if (parameters.DegreesX == 90) return 180;
            return 0;
        case 90:
            if (parameters.DegreesX == 0) return -90;
            if (parameters.DegreesX == 180) return 90;
            if (parameters.DegreesX == 270) return 0;
            return 180;
        case 180:
            if (parameters.DegreesX == 270) return 0;
            return 180;
        default:
            if (parameters.DegreesX == 0) return 90;
            if (parameters.DegreesX == 180) return -90;
            if (parameters.DegreesX == 270) return 0;
            return 180;
        }
    }

    int AdjustSizeTop(const ModelParameters& parameters) const {
        switch (parameters.DegreesY)
        {
        case 0:
            if (parameters.DegreesX == 270) return 180;
            return 0;
        case 90:
            if (parameters.DegreesX == 0) return 90;
            if (parameters.DegreesX == 180) return -90;
            if (parameters.DegreesX == 270) return 180;
            return 0;
        case 180:
            if (parameters.DegreesX == 270 || parameters.DegreesX == 180) return 180;
            return 0;
        default:
            if (parameters.DegreesX == 0) return -90;
            else if (parameters.DegreesX == 180) return 90;
            else if (parameters.DegreesX == 270) return 180;
            return 0;
        }
    }

    inline int Mod(int x, int m) const {
        return (x % m + m) % m;
    }

    Uv GenerateUv(const ElementToken& element, Axis axis) const {
        switch (axis)
        {
        case Axis::X:
            return Uv(
                Vec2f(element.From.Z, 16 - element.To.Y),
                Vec2f(element.To.Z, 16 - element.From.Y));
        case Axis::Y:
            return Uv(
                Vec2f(element.From.X, element.From.Z),
                Vec2f(element.To.X, element.To.Z));
        default:
            return Uv(
                Vec2f(element.From.X, 16 - element.To.Y),
                Vec2f(element.To.X, 16 - element.From.Y));
        }
    }

    void GetTexturePath(const std::string& const value, const ModelToken& model, std::string& output) const {
        if (value.empty()) return;

        const std::string SKIP = "minecraft:";

        if (String::StartsWith(value, SKIP)) output = value.substr(SKIP.size());
        if (String::StartsWith(value, "#")) {
            std::string removedPrefix = value.substr(1);
            if (model.Textures.find(removedPrefix) == model.Textures.end()) return;

            std::string v = model.Textures.at(removedPrefix);
            if (value == v) return;

            output = v;
            GetTexturePath(v, model, output);
        }
    }

    ReadonlyImage* ProvideImage(const std::string& path, UvImageParameters parameters) const {
        LockedBitmap* bitmap = ProvideBitmap(path);
        if (bitmap == NULL) return Empty;

        Animation* animation = AnimationProv->Provide(path, UriKind::Absolute, bitmap);

        UvReadonlyImage* output = new UvReadonlyImage(bitmap, parameters, animation);
        output->TickProv = TickTextureProvider;

        return output;
    }

    LockedBitmap* ProvideBitmap(const std::string& path) const {
        LockedBitmap* bitmap = NULL;
        if (!path.empty()) {
            bitmap = ImageProv->Provide(path, UriKind::Absolute);
        }

        if (path.empty() || bitmap == NULL) {
            bitmap = ImageProv->Provide("missing_texture", UriKind::Relative);
        }

        return bitmap;
    }
};

