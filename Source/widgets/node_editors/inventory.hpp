#pragma once
#include "node_editor.hpp"
#include "cpinternals/cpnames.hpp"
#include "cserialization/cnodes/CInventory.hpp"
#include "itemData.hpp"


// to be used with CInventory struct
struct inventory_widget
{
  //static CItemData copied_item = ;

  // returns true if content has been edited
  [[nodiscard]] static inline bool draw(CInventory& inv, bool* p_remove = nullptr)
  {
    scoped_imgui_id _sii("##inventory_editor");
    bool modified = false;

    for (auto inv_it = inv.m_subinvs.begin(); inv_it != inv.m_subinvs.end(); ++inv_it)
    {
      ImGuiWindow* window = ImGui::GetCurrentWindow();
      ImGuiID row_id = window->GetID(&*inv_it);

      auto& subinv = *inv_it;

      std::stringstream ss;
      ss << "inventory_" << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << subinv.uid;
      if (ImGui::TreeNodeBehavior(row_id, ImGuiTreeNodeFlags_Framed, ss.str().c_str()))
      {
        if (ImGui::Button("Add dummy item (alcohol6)", ImVec2(0, 30)))
        {
          // todo: move that on the data side
          CItemData item_data;
          item_data.iid.nameid.as_u64 = 0x1859EA0850; // Alcohol6
          item_data.iid.uk.uk4 = 2;
          item_data.uk1_012 = 0x213ACD;
          item_data.uk2_01 = 1; // quantity
          subinv.items.insert(subinv.items.begin(), 1, item_data);
          modified = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Unflag all Quest items (makes them normal items)", ImVec2(0, 30)))
        {
          for (auto& item : subinv.items)
            item.uk0_012 &= 0xFE;
          modified = true;
        }

        for (auto it = subinv.items.begin(); it != subinv.items.end();)
        {
          scoped_imgui_id _sii(&*it);
          bool torem = false;

          modified |= itemData_widget::draw(*it, &torem);
          if (torem) {
            it = subinv.items.erase(it);
            modified = true;
          }
          else
            ++it;
        }

        ImGui::TreePop();
      }
    }

    return modified;
  }
};


class inventory_editor
  : public node_editor_widget
{
  CInventory m_inv;

public:
  inventory_editor(const std::shared_ptr<const node_t>& node)
    : node_editor_widget(node)
  {
    reload();
  }

  ~inventory_editor() override {}

public:
  bool commit_impl() override
  {
    auto rebuilt = m_inv.to_node();
    auto curnode = ncnode();

    if (!rebuilt)
      return false;

    curnode->assign_children(rebuilt->children());
    curnode->assign_data(rebuilt->data());
    return true;
  }

  bool reload_impl() override
  {
    bool success = m_inv.from_node(node());
    return success;
  }

protected:
  void draw_impl(const ImVec2& size) override
  {
    scoped_imgui_id _sii(this);

    bool changes = inventory_widget::draw(m_inv, 0);
    if (changes)
      m_has_unsaved_changes = true;
  }
};

