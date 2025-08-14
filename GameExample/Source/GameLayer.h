#pragma once  
#include <Rubber.h>

class GameLayer : public RB::Layer {  

public:  
   GameLayer() = default;  

   void onAttach() override;  

   void onDetach()  override;;  

   void onUpdate(float ts) override;;  

   void onImGuiRender() override;;  

private:
   void LoadAllAsset(const RB::Ref<RB::AssetManager>& assetManager);  

private:  
   RB::Ref<RB::Scene> m_ActiveScene;  
   RB::Ref<RB::Asset> m_Asset;  
   RB::Entity m_Player;  
   RB::Entity m_Enemy;
};
