#include "pch.h"
#include "AnimationSystem.h"

#include "Rubber/Core/Application.h"
#include "Rubber/Scene/Utili/Component.h"

#include "Rubber/Resources/Asset.h"
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AssetMetaData.h"
#include "Rubber/Resources/AnimationResources/AnimationAtlas.h"
#include "Rubber/Resources/AnimationResources/AnimationSpriteSheet.h"

namespace Rubber {

	static void initAnimationTimer(entt::registry& registry, entt::entity e);
	static Ref<SubTexture2D> updateSpriteTexture(const AnimationComponent& animationCC, const SpriteComponent& spriteCC);

	void AnimaitonSystem::init(Scene* scene)
	{
		SystemBase::init(scene);
		m_Registry->on_construct<AnimationComponent>().connect<&initAnimationTimer>();
	}


	void AnimaitonSystem::onUpdate(float ts)
	{
		{
			auto view = m_Registry->view<AnimationComponent, SpriteComponent>();

			for (auto entity : view) {
				auto& spriteCC = view.get<SpriteComponent>(entity);
				auto& animationCC = view.get<AnimationComponent>(entity);
				if (!animationCC.specs.m_ActiveAnimationClip) continue;
				
				AnimationTimer& timer = animationCC.specs.m_Timer;
				timer.onUpdate(ts); 
				// TODO::WHEN TIMER IS EXPIRED, don't bother to selecte correct texture anymore, not rendered anyway
				
				Ref<SubTexture2D> finalTextureToRender = updateSpriteTexture(animationCC, spriteCC);
				spriteCC.texture = finalTextureToRender;

				// calculate the local offset based on the anchor mode and store in sprite
				glm::vec2 textureDimension = spriteCC.texture->getSubtextureDimension();
				if (animationCC.specs.m_AnchorMode == AnimationSpec::AnchorMode::CENTERED) {
					spriteCC.localOffset = { 0.0f, 0.0f }; // no offset needed, render anchor is centered by default
				}
				else { // bottom center, x value stays the same
					spriteCC.localOffset.x = 0.0f ; 
					spriteCC.localOffset.y = 0.5f ;
	 			}
			}
		}
	}

	static void initAnimationTimer(entt::registry& registry, entt::entity e) {
		auto& animationCC = registry.get<AnimationComponent>(e);
		auto& animationTimer = animationCC.specs.m_Timer;
		animationTimer.runOnlyOnce(false);
		animationCC.specs.m_Timer.setOnTimeOut([&](){
			AnimationSpec& specs = animationCC.specs;
			specs.m_FrameIndex++;
			if (specs.m_FrameIndex >= specs.m_FrameCount) {
				specs.m_FrameIndex = (specs.m_IsLoop) ? 0 : specs.m_FrameCount - 1;
				if (!specs.m_IsLoop && specs.m_OnFinished) {
					specs.m_OnFinished();
				}
			}
		});
	}


	static Ref<SubTexture2D> updateSpriteTexture(const AnimationComponent& animationCC, const SpriteComponent& spriteCC ) {
		AssetHandle currentAnimationSrc = animationCC.specs.m_ActiveAnimationClip;
		const Ref<AssetManager>& assetManager = Application::getAssetManager();
		const AssetMetaData& metadata = assetManager->getAssetMetadata(currentAnimationSrc);

		Ref<SubTexture2D> finalTextureToRender = SubTexture2D::create();
		uint32_t frameIndex = animationCC.specs.m_FrameIndex;
		bool isOppositeDireciton = animationCC.specs.m_IsOpposizeDireciton;

		const Ref<Asset>& asset = assetManager->getAsset(currentAnimationSrc);
		switch (metadata.type) {
				case AssetType::AnimationAtlas:
				{
					const Ref<AnimationAtlas>& atlas = static_pointer_cast<AnimationAtlas> (asset);
					finalTextureToRender = SubTexture2D::createFromTexture(atlas->getTextureAtIndex(frameIndex), isOppositeDireciton);
					break;
				}
				case AssetType::AnimationSpriteSheet:
				{
					const Ref<AnimationSpriteSheet>& spriteSheet = static_pointer_cast<AnimationSpriteSheet> (asset);
					finalTextureToRender = spriteSheet->getSubTextureAt(frameIndex, 0, isOppositeDireciton);
					break;
				}
				case AssetType::Texture2D:
				{
					const Ref<Texture2D>& texture2D = static_pointer_cast<Texture2D> (asset);
					finalTextureToRender = SubTexture2D::createFromTexture(texture2D, isOppositeDireciton);
					break;
				}
				default:
				{
					RB_CORE_ASSERT(false, "Not a supported animation src format!");
					break;
				}
			}
			return finalTextureToRender;
	}
}

