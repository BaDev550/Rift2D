#pragma once
#include <Editor/EditorPanel.h>

RCLASS();
class AssetBrowserWidget final : public EditorPanel {
	GENERATE_CLASS(AssetBrowserWidget);
public:
	AssetBrowserWidget() = default;
	~AssetBrowserWidget() final = default;

	virtual void OnGUI() final;
	virtual const char* GetTitle() const final { return "AssetBrowser"; }
private:
	char _searchFilter[128] = {};
	bool _onlyShowLoaded = false;
};