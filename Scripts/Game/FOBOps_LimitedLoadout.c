//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true), BaseContainerCustomTitleField("m_sLoadoutName")]
class FOBOps_LimitedLoadout : SCR_FactionPlayerLoadout
{
	[Attribute(defvalue: "1", UIWidgets.Slider, desc: "Maximum number of loadouts allowed.", params: "1 64 1")]
	protected int m_inumberLoadout;
	
	override bool IsLoadoutAvailable(int playerId)
	{
		CountLoadouts();
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsLoadoutAvailableClient()
	{
		CountLoadouts();
		return true;
	}
	
	protected void CountLoadouts()
	{
		int loadoutCount = 0;
		array<int> PlayersArray = new array<int>();
		GetGame().GetPlayerManager().GetPlayers(PlayersArray);
		foreach(int PlayerId : PlayersArray )
		{
			IEntity PlayerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(PlayerId);
			SCR_ChimeraCharacter PlayerCharacter = SCR_ChimeraCharacter.Cast(PlayerEntity);
			Print(m_sLoadoutResource);
			Print(PlayerCharacter);
		}
	}
}