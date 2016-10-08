#include "esp.h"

bool Settings::ESP::enabled	= true;
Color Settings::ESP::ally_color = Color(0, 50, 200);
Color Settings::ESP::enemy_color = Color(200, 0, 50);
Color Settings::ESP::enemy_visible_color = Color(200, 200, 50);
Color Settings::ESP::bones_color = Color(255, 255, 255);
Color Settings::ESP::bomb_color = Color(200, 0, 50);
bool Settings::ESP::visibility_check = false;
bool Settings::ESP::Walls::enabled = true;
bool Settings::ESP::Info::showName = true;
bool Settings::ESP::Info::showHealth = true;
bool Settings::ESP::Bones::enabled = true;
bool Settings::ESP::Bomb::enabled = true;
bool Settings::ESP::Tracer::enabled = false;
TracerType Settings::ESP::Tracer::type = BOTTOM;

bool WorldToScreen(const Vector &vOrigin, Vector &vScreen)
{
	return debugOverlay->ScreenPosition(vOrigin, vScreen);
}

Vector2D WorldToScreen(const Vector &vOrigin)
{
	Vector vec;
	debugOverlay->ScreenPosition(vOrigin, vec);
	return LOC(vec.x, vec.y);
}

static wchar_t* cwConvert(const char* text)
{
	const size_t size = strlen(text) + 1;
	wchar_t* wText = new wchar_t[size];
	mbstowcs(wText, text, size);
	return wText;
}

void DrawESPBox(Vector vecOrigin, Vector vecViewOffset, Color color, int width, int additionalHeight)
{
	//SIDES
	Vector2D a = WorldToScreen(vecOrigin + Vector(width, width, additionalHeight));
	Vector2D b = WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(-width, width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(-width, -width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(width, -width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);

	// TOP
	a = WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight));
	b = WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);

	// BOTTOM
	a = WorldToScreen(vecOrigin + Vector(width, width, additionalHeight));
	b = WorldToScreen(vecOrigin + Vector(width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(width, width, additionalHeight));
	b = WorldToScreen(vecOrigin + Vector(-width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(-width, -width, additionalHeight));
	b = WorldToScreen(vecOrigin + Vector(-width, width, additionalHeight));
	Draw::DrawLine(a, b, color);

	a = WorldToScreen(vecOrigin + Vector(width, -width, additionalHeight));
	b = WorldToScreen(vecOrigin + Vector(-width, -width, additionalHeight));
	Draw::DrawLine(a, b, color);
}

void ESP::DrawBones(C_BaseEntity* entity)
{
	studiohdr_t* pStudioModel = modelInfo->GetStudioModel(entity->GetModel());
	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];
	if (entity->SetupBones(pBoneToWorldOut, 128, 256, 0))
	{
		for (int i = 0; i < pStudioModel->numbones; i++)
		{
			mstudiobone_t* pBone = pStudioModel->pBone(i);
			if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
				continue;

			Vector vBonePos1;
			if (WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
				continue;

			Vector vBonePos2;
			if (WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
				continue;

			Draw::DrawLine(LOC(vBonePos1.x, vBonePos1.y), LOC(vBonePos2.x, vBonePos2.y), Settings::ESP::bones_color);
		}
	}
}

void ESP::DrawTracer(C_BasePlayer* localPlayer, C_BaseEntity* entity)
{
	int playerTeam = localPlayer->GetTeam();
	int entityTeam = reinterpret_cast<C_BasePlayer*>(entity)->GetTeam();
	Color color = playerTeam == entityTeam ? Settings::ESP::ally_color : Settings::ESP::enemy_color;

	int width;
	int height;
	engine->GetScreenSize(width, height);
	
	Vector2D tracerLocation;
	
	switch (Settings::ESP::Tracer::type)
	{
		case BOTTOM:
			tracerLocation = LOC(width / 2, height);
			break;
		case CURSOR:
			tracerLocation = LOC(width / 2, height / 2);
			break;
	}
	
	Vector s_vecEntity_s;
	if (!WorldToScreen(entity->GetVecOrigin(), s_vecEntity_s) && localPlayer->GetHealth() > 0)
		Draw::DrawLine(tracerLocation, LOC(s_vecEntity_s.x, s_vecEntity_s.y), color);
}

void ESP::DrawPlayerBox(C_BasePlayer* localPlayer, C_BaseEntity* entity)
{
	Color color;

	int playerTeam = localPlayer->GetTeam();
	int entityTeam = reinterpret_cast<C_BasePlayer*>(entity)->GetTeam();
	bool isVisible = Entity::IsVisible(localPlayer, entity, BONE_HEAD);

	if (playerTeam != entityTeam)
	{
		color = isVisible ? Settings::ESP::enemy_visible_color : Settings::ESP::enemy_color;
	}
	else
	{
		color = Settings::ESP::ally_color;
	}
	
	int width = 14;
	int additionalHeight = 6;
	
	Vector vecOrigin = entity->GetVecOrigin();
	
	Vector vecHeadBone = entity->GetBonePosition(Bones::BONE_HEAD);
	Vector vecViewOffset = Vector(vecOrigin.x, vecOrigin.y, vecHeadBone.z);
	
	Vector s_vecLocalPlayer_s;
	if (!WorldToScreen(vecOrigin, s_vecLocalPlayer_s))
		DrawESPBox(vecOrigin, vecViewOffset, color, width, additionalHeight);
}

void ESP::DrawPlayerInfo(C_BasePlayer* localPlayer, C_BaseEntity* entity, int entityIndex)
{
	int playerTeam = localPlayer->GetTeam();
	int entityTeam = reinterpret_cast<C_BasePlayer*>(entity)->GetTeam();
	Color color = playerTeam == entityTeam ? Settings::ESP::ally_color : Settings::ESP::enemy_color;
	
	IEngineClient::player_info_t entityInformation;
	engine->GetPlayerInfo(entityIndex, &entityInformation);
	
	Vector vecOrigin = entity->GetVecOrigin();
	
	pstring str;
	
	if (Settings::ESP::Info::showName)
		str << entityInformation.name;

	if (Settings::ESP::Info::showName && Settings::ESP::Info::showHealth)
		str << " ";

	if (Settings::ESP::Info::showHealth)
	{
		str + entity->GetHealth();
		str << "hp";
	}
	
	std::wstring wstr(str.begin(), str.end());
	
	Vector s_vecEntity_s;
	if (!WorldToScreen(vecOrigin, s_vecEntity_s))
		Draw::DrawString(wstr.c_str(), LOC(s_vecEntity_s.x, s_vecEntity_s.y), color, esp_font, true);
}

void ESP::DrawBombBox(C_BaseEntity* entity)
{
	Color color = Settings::ESP::bomb_color;

	int width = 7;
	int additionalHeight = 4;

	Vector vecOrigin = entity->GetVecOrigin();
	Vector vecViewOffset = Vector(vecOrigin.x, vecOrigin.y, vecOrigin.z - 4);

	Vector s_vecLocalPlayer_s;
	if (!WorldToScreen(vecOrigin, s_vecLocalPlayer_s))
	{
		DrawESPBox(vecOrigin, vecViewOffset, color, width, additionalHeight);
		Draw::DrawString(L"C4", LOC(s_vecLocalPlayer_s.x, s_vecLocalPlayer_s.y), color, esp_font, true);
	}
}

void ESP::PaintTraverse(VPANEL vgui_panel, bool force_repaint, bool allow_force)
{
	if (!Settings::ESP::enabled)
		return;

	if (!engine->IsInGame())
		return;

	C_BasePlayer* localPlayer = (C_BasePlayer*)entitylist->GetClientEntity(engine->GetLocalPlayer());

	if (!localPlayer)
		return;

	for (int i = 0; i < entitylist->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = entitylist->GetClientEntity(i);
		if (!entity)
			continue;

		ClientClass* client = entity->GetClientClass();
		if (client->m_ClassID == CCSPlayer)
		{
			if (entity == (C_BaseEntity*)localPlayer
				|| entity->GetDormant()
				|| entity->GetLifeState() != LIFE_ALIVE
				|| entity->GetHealth() <= 0)
				continue;

			if (Settings::ESP::visibility_check && !Entity::IsVisible(localPlayer, entity, 6))
				continue;

			if (Settings::ESP::Bones::enabled)
				ESP::DrawBones(entity);

			if (Settings::ESP::Walls::enabled)
				ESP::DrawPlayerBox(localPlayer, entity);

			if (Settings::ESP::Tracer::enabled)
				ESP::DrawTracer(localPlayer, entity);

			ESP::DrawPlayerInfo(localPlayer, entity, i);
		}
		else if (client->m_ClassID == CPlantedC4)
		{
			if (Settings::ESP::Bomb::enabled && entity->IsBombTicking())
				ESP::DrawBombBox(entity);
		}
	}
}