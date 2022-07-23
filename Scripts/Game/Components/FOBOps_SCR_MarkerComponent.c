[EntityEditorProps(category: "GameScripted/Components", description: "Base lock component.")]
class FOBOps_SCR_MarkerComponentClass : ScriptComponentClass
{
};

//------------------------------------------------------------------------------------------------
class FOBOps_SCR_MarkerComponent : ScriptComponent
{
	override void OnPostInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(SetMarker, 1000, false, owner);
		
	}
	
	protected void SetMarker(IEntity owner)
	{
		SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(owner.FindComponent(SCR_MapDescriptorComponent));
        if (!mapDescriptorComponent) return;
            protected MapItem m_MapItem = mapDescriptorComponent.Item();
		if (!m_MapItem) return;
            MapDescriptorProps props = m_MapItem.GetProps();
		if (!props) return;
            m_MapItem.SetProps(props);
            props.SetFrontColor(Color.Red);
            m_MapItem.SetBaseType(EMapDescriptorType.MDT_ICON);
            props.SetImageDef("11-5");
			props.SetIconSize(0.25,0.3,1);
			//vector crateLoc = m_MapItem.GetPos();
			//int newX = Math.RandomInt(-150, 150)+crateLoc[0];
			//int newZ = Math.RandomInt(-150, 150)+crateLoc[2];
			//m_MapItem.SetPos(newX,newZ);
			//m_MapItem.SetVisible(true);
            props.Activate(true);
	}
}