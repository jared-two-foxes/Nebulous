
#include "WidgetRenderer.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>


#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/Control/EditControl.h>
#include <Nebulae/Beta/Gui/Control/ListBox.h>
#include <Nebulae/Beta/Gui/Control/Slider.h>
#include <Nebulae/Beta/Gui/Control/StaticGraphic.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Widget.h>

#include <Nebulae/Beta/Font/FontRenderer.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/SpriteBatch/SpriteBatch.h>

using namespace Nebulae;

namespace {

  //@note [jared.watt] Used for ListBox rendering. 
  bool LessThanEqual( ListBox::const_iterator lhs, ListBox::const_iterator rhs, ListBox::const_iterator end )
  {
    return lhs == rhs||
      ListBox::RowPtrIteratorLess<std::list<ListBox::Row*> >::LessThan(lhs, rhs, end);
  }

  const float g_fRectVertices[] = { 0.0f,0.0f, 1.0f,0.0f, 0.0,1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f };
}



WidgetRenderer::WidgetRenderer( FileArchivePtr fileSystem, SpriteBatch* batcher, RenderSystemPtr renderSystem )
: OverlayRenderer( renderSystem, batcher ), 
  m_fileSystem( fileSystem ),
  m_activeScheme( nullptr ),
  m_focusWidget( nullptr )
{}


FontRenderer*
WidgetRenderer::AddFont( const std::shared_ptr<Font >& font ) 
{
  FontRenderer* fontRenderer = new FontRenderer( m_renderDevice, m_batcher, font );
  
  File* is = m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, font->FontName() );
  if( is != nullptr ) 
  {
    fontRenderer->Init( is ); 
  }
  
  m_fontRenderers.push_back( fontRenderer );

  return fontRenderer;
}

void
WidgetRenderer::SetFocusWindow( Widget* focusWidget )
{
  m_focusWidget = focusWidget;
}

void
WidgetRenderer::Render( const std::list<Widget* >& widgets )
/// 
/// Iterates all of the widgets contained in the system and renders them using the current
/// RenderDevice object.
///
/// @param system
///   The ui system to render.
///
/// @return
///   Nothing.
///
{
  const RenderSystem::WindowPtr window = m_renderDevice->GetWindow();
  NE_ASSERT( window, "Unable to retrieve the window object that the RenderDevice is bound to." )();
  if( !window || widgets.empty() ) return;
  
  const Real fLeft   = 0.0f;
  const Real fRight  = (Real)window->GetWidth();
  const Real fBottom = 0.0f;
  const Real fTop    = (Real)window->GetHeight();

  // Setup bounds that we wish to be visible.
  const Real fNear   = 0.001f;
  const Real fFar    = 1000.0f;

  // Create the ortho matrix.
  Matrix4 projection;
  projection.MakeOrthoMatrix( fLeft, fRight, fBottom, fTop, fNear, fFar );

  // Grab the material that we are using.
  if( m_basicMaterial != NULL ) 
  {
    Pass* pass = m_basicMaterial->GetPass( 0 );

    m_renderDevice->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

    UniformDefinition projectionDef = m_renderDevice->GetUniformByName( "projection" );
    m_renderDevice->SetUniformBinding( projectionDef, projection.ptr() );
  }

  if( m_complexMaterial != NULL ) 
  {
    Pass* pass = m_complexMaterial->GetPass( 0 );

    m_renderDevice->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

    UniformDefinition projectionDef = m_renderDevice->GetUniformByName( "projection" );
    m_renderDevice->SetUniformBinding( projectionDef, projection.ptr() );
  }

  m_renderDevice->SetBlendingState( true );
  m_renderDevice->SetDepthTest( false );

  for( std::list<Widget* >::const_iterator it = widgets.begin(); 
       it != widgets.end(); 
       ++it ) 
  {
    RenderWidget( *(*it) );
  }

	if ( m_batcher )
  {
		m_batcher->Draw();
	}

  m_renderDevice->SetBlendingState( false );
  m_renderDevice->SetDepthTest( true );
}

const FontRenderer*
WidgetRenderer::FindFontRendererByFont( const std::shared_ptr<Font >& font ) const 
{
  std::vector<FontRenderer* >::const_iterator itr, end_itr = m_fontRenderers.end();
  for( itr = m_fontRenderers.begin(); itr != end_itr; ++itr ) 
  {
    if( (*itr)->GetFont() == font ) 
    {
      return (*itr);
    }
  }

  return NULL;
}

void 
WidgetRenderer::BeginClipping( const Widget& widget )
{
  //switch( mode ) {
  //  case DontClip:
  //    assert(!"Wnd::BeginClippingImpl() called with mode == DontClip; this should never happen.");
  //    break;
  //  case ClipToClient:
  //  case ClipToClientAndWindowSeparately:
  //    m_renderDevice->BeginScissorClipping( widget.GetClientUpperLeft(), widget.GetClientLowerRight() );
  //    break;
  //  case ClipToWindow:
      m_renderDevice->BeginScissorClipping( widget.GetUpperLeft(), widget.GetLowerRight() );
  //    break;
  //}
}

void 
WidgetRenderer::EndClipping( const Widget& widget )
{
  //switch (mode) {
  //  case DontClip:
  //    assert(!"Wnd::EndClippingImpl() called with mode == DontClip; this should never happen.");
  //    break;
  //  case ClipToClient:
  //  case ClipToWindow:
  //  case ClipToClientAndWindowSeparately:
      m_renderDevice->EndScissorClipping();
  //    break;
  //}
}

void 
WidgetRenderer::RenderWidget( const Widget& widget )
{
  if( widget.IsVisible() ) 
  {
  //
  // Determine the depth that the widget should be rendered.  The depth is 
  // offset 30 bits into the int.  See the ZList for details.  We then offset
  // the deth by 8 and negate.
  //
    int32 depth = widget.GetDepth();
    depth >>= 30;
    depth += 8;
    depth = -depth;

  //
  // Render this widget.  Dependent upon widget type.
  //
    switch( widget.GetWidgetType() ) 
    {
      case WIDGET: 
      case CONTROL: 
        break;
      
      case LABEL: 
        {
          const TextControl* control = static_cast<const TextControl*>(&widget);
          RenderTextControl( *control, depth );
        }
        break;

      case EDIT: 
        {
          const EditControl* control = static_cast<const EditControl*>(&widget);
          RenderEditControl( *control, depth );
        }
        break;
 
      case STATIC: 
        {
          const StaticGraphic* control = static_cast<const StaticGraphic*>(&widget);
          const SubTexture*    graphic = control->GetGraphic();

          DrawQuad( m_renderDevice, widget.GetUpperLeft(), widget.GetLowerRight(), control->GetColour(), graphic, depth );
        }
        break;

      case BUTTON: 
        {
          const ButtonControl*     button  = static_cast<const ButtonControl*>(&widget);
          RenderButton( *button, depth );
        }
        break;

      case SLIDER: 
        {
          const Slider* slider      = static_cast<const Slider*>(&widget);
          RenderSlider( *slider, depth );
        }
        break;

      case LISTBOX: 
      {
        const ListBox* listbox = static_cast<const ListBox*>(&widget);
        RenderListBox( *listbox, depth );
      }
      break;
    }
    
  //
  // Render any child widgets.
  //
    const std::list<Widget*>& children = widget.GetChildren();
    for( std::list<Widget*>::const_iterator it = children.begin(), end = children.end(); it != end; ++it ) 
    {
      RenderWidget( *(*it) );
    }
  }
}

void 
WidgetRenderer::RenderButton( const ButtonControl& control, int32 depth )
{
  const SubTexture* texture = nullptr;
  if( control.GetState() == ButtonControl::BN_UNPRESSED ) 
  {
    texture = control.GetUnpressedGraphic();
  } 
  else if( control.GetState() == ButtonControl::BN_PRESSED ) 
  {
    texture = control.GetPressedGraphic();
  }

  if( texture ) 
  {
    DrawQuad( m_renderDevice, control.GetUpperLeft(), control.GetLowerRight(), control.GetColour(), texture, depth );
  }
  else 
  {
    BeveledRectangle( control.GetUpperLeft(), control.GetLowerRight(),
                      control.IsDisabled() ? GetDisabledColour(control.GetColour()) : control.GetColour(),
                      control.IsDisabled() ? GetDisabledColour(control.GetColour()) : control.GetColour(),
                      (control.GetState() != ButtonControl::BN_PRESSED), 1);
  }

  if( control.GetText().length() > 0 )
  {
    RenderTextControl( control, depth );
  }
}

void 
WidgetRenderer::RenderTextControl( const TextControl& control, int32 depth )
{
  const Colour                      clr_to_use  = control.IsDisabled() ? GetDisabledColour(control.TextColor()) : control.TextColor();
  Flags<TextFormat>                 text_format = control.GetTextFormat();
  const std::vector<Font::LineData> line_data   = control.GetLineData();

  if( control.GetFont() != NULL ) 
  {
    // This is probably less than ideal and it requires that the render functions be non-const
    // but at this point it's the easiest option.
    const FontRenderer* fontRenderer = FindFontRendererByFont( control.GetFont() );
    if( fontRenderer == nullptr ) 
    {
      fontRenderer = AddFont( control.GetFont() );
    }

    if( control.ClipText() ) 
    {
      BeginClipping( control );
    }

    fontRenderer->RenderText( control.GetUpperLeft(), control.GetLowerRight(), control.GetText(), text_format, clr_to_use, &line_data );
            
    if( control.ClipText() ) 
    {
      EndClipping( control );
    }
  }
}

void 
WidgetRenderer::RenderEditControl( const EditControl& control, int32 depth )
{
  Colour color_to_use          = control.IsDisabled() ? GetDisabledColour(control.GetColour()) : control.GetColour();
  Colour int_color_to_use      = control.IsDisabled() ? GetDisabledColour(control.InteriorColour()) : control.InteriorColour();
  Colour sel_text_color_to_use = control.IsDisabled() ? GetDisabledColour(control.SelectedTextColour()) : control.SelectedTextColour();
  Colour hilite_color_to_use   = control.IsDisabled() ? GetDisabledColour(control.HiliteColour()) : control.HiliteColour();
  Colour text_color_to_use     = control.IsDisabled() ? GetDisabledColour(control.TextColor()) : control.TextColor();

  Point ul        = control.GetUpperLeft(), lr = control.GetLowerRight();
  Point client_ul = control.GetClientUpperLeft(), client_lr = control.GetClientLowerRight();

  // This is probably less than ideal and it requires that the render functions be non-const
  // but at this point it's the easiest option.
  const FontRenderer* fontRenderer = FindFontRendererByFont( control.GetFont() );
  if( fontRenderer == nullptr ) 
  {
    fontRenderer = AddFont( control.GetFont() );
  }

  BeveledRectangle( ul, lr, color_to_use/*int_color_to_use*/, color_to_use, false, 2 );

  //BeginScissorClipping( Point(client_ul.x - 1, client_ul.y), client_lr );

  const std::vector<Font::LineData::CharData>& char_data = control.GetLineData()[0].char_data;
  int32 first_char_offset = control.FirstCharOffset();
  int32 text_y_pos(ul.y + ((lr.y - ul.y) - control.GetFont()->Height()) / 2.0 + 0.5);
  CPSize last_visible_char = control.LastVisibleChar();
  const StrSize INDEX_0   = StringIndexOf(0, control.FirstCharShown(), control.GetLineData());
  const StrSize INDEX_END = StringIndexOf(0, last_visible_char, control.GetLineData());
  if( control.MultiSelected() ) // if one or more chars are selected, hilite, then draw the range in the selected-text color
  {
    CPSize low_cursor_pos  = std::min(control.CursorPosn().first, control.CursorPosn().second);
    CPSize high_cursor_pos = std::max(control.CursorPosn().first, control.CursorPosn().second);

    // draw hiliting
    Point hilite_ul(client_ul.x + (low_cursor_pos < 1 ? 0 : char_data[Value(low_cursor_pos - 1)].extent) - first_char_offset, client_ul.y);
    Point hilite_lr(client_ul.x + char_data[Value(high_cursor_pos - 1)].extent - first_char_offset, client_lr.y);
    FlatRectangle(hilite_ul, hilite_lr, hilite_color_to_use, CLR_ZERO, 0);

    // INDEX_0 to INDEX_1 is unhilited, INDEX_1 to
    // INDEX_2 is hilited, and INDEX_2 to INDEX_3 is
    // unhilited; each range may be empty
    const StrSize INDEX_1 = StringIndexOf(0, std::max(low_cursor_pos, control.FirstCharShown()), control.GetLineData());
    const StrSize INDEX_2 = StringIndexOf(0, std::min(high_cursor_pos, last_visible_char), control.GetLineData());

    // draw text
    int32 text_x_pos = ul.x + EditControl::PIXEL_MARGIN;
    
    // TODO: Use subrange RenderText() 

    text_x_pos += fontRenderer->RenderText( Point(text_x_pos, text_y_pos), control.GetText().substr(Value(INDEX_0), Value(INDEX_1 - INDEX_0)), text_color_to_use );

    text_x_pos += fontRenderer->RenderText( Point(text_x_pos, text_y_pos), control.GetText().substr(Value(INDEX_1), Value(INDEX_2 - INDEX_1)), text_color_to_use );
    text_x_pos += fontRenderer->RenderText( Point(text_x_pos, text_y_pos), control.GetText().substr(Value(INDEX_2), Value(INDEX_END - INDEX_2)), text_color_to_use );
  }
  else  // no selected text
  { 
    fontRenderer->RenderText( Point(client_ul.x, text_y_pos), control.GetText().substr(Value(INDEX_0), Value(INDEX_END - INDEX_0)), text_color_to_use );

    if( m_focusWidget == &control )
    {
      //int32 caret_x = control.ScreenPosOfChar( m_cursor_pos.second );
      //glDisable(GL_TEXTURE_2D);
      //glBegin(GL_LINES);
      //glVertex(caret_x, client_ul.y);
      //glVertex(caret_x, client_lr.y);
      //glEnd();
      //glEnable(GL_TEXTURE_2D);
    }
  }

  //EndScissorClipping();
}

void
WidgetRenderer::RenderListBox( const ListBox& control, int32 depth )
{
  // draw beveled rectangle around client area
  Point ul    = control.GetUpperLeft(), 
        lr    = control.GetLowerRight();
  Point cl_ul = control.GetClientUpperLeft(), 
        cl_lr = control.GetClientLowerRight();
  Colour color_to_use        = control.IsDisabled() ? GetDisabledColour(control.GetColour()) : control.GetColour();
  Colour int_color_to_use    = control.IsDisabled() ? GetDisabledColour(control.InteriorColour()) : control.InteriorColour();
  Colour hilite_color_to_use = control.IsDisabled() ? GetDisabledColour(control.HiliteColour()) : control.HiliteColour();

  BeveledRectangle(ul, lr, int_color_to_use, color_to_use, false, ListBox::BORDER_THICK);

  if( control.FirstRowShown() == control.end() ) 
  {
    return;
  }

  ListBox::iterator last_visible_row = control.LastVisibleRow();

  //@note [jared.watt] No clipping mode yet.
  BeginClipping( control );

//
// draw selection highlighting
//
// Note that inside the for loop below, prev_sel is guaranteed to be valid
// (i.e. != m_rows.end()), since a non-empty m_selections implies a
// non-empty m_rows, and a non-empty m_rows implies a valid
// m_first_row_shown.
//
  ListBox::iterator prev_sel = control.FirstRowShown();
  int top(0);
  int bottom = (*prev_sel)->GetHeight();
  for( ListBox::SelectionSet::iterator sel_it = control.Selections().begin(); sel_it != control.Selections().end(); ++sel_it ) 
  {
    ListBox::iterator curr_sel = *sel_it;
    if( LessThanEqual(control.FirstRowShown(), curr_sel, control.end() ) &&
        LessThanEqual(curr_sel, last_visible_row, control.end()) ) 
    {
      // No need to look for the current selection's top, if it is the
      // same as the bottom of the last iteration.
      if( boost::next(prev_sel) == curr_sel ) 
      {
        top = bottom;
      } 
      else 
      {
        for( ListBox::iterator it = prev_sel; it != curr_sel; ++it ) 
        {
          top += (*it)->GetHeight();
        }
      }
      bottom = top + (*curr_sel)->GetHeight();
      if( cl_lr.y < bottom ) 
      {
        bottom = cl_lr.y;
      }
      FlatRectangle( Point(cl_ul.x, cl_ul.y + top), Point(cl_lr.x, cl_ul.y + bottom),
                      hilite_color_to_use, CLR_ZERO, 0 );
      prev_sel = curr_sel;
    }
  }

//
// draw caret
//
  if( control.Caret() != control.end() &&
      LessThanEqual(control.FirstRowShown(), control.Caret(), control.end()) &&
      LessThanEqual(control.Caret(), last_visible_row, control.end()) /*&&
      MatchesOrContains(&control, GUI::GetGUI()->FocusWnd())*/ ) 
  {
    Point row_ul = (*control.Caret())->GetUpperLeft();
    Point row_lr = (*control.Caret())->GetLowerRight();
    row_lr.x = control.GetClientLowerRight().x;
    FlatRectangle( row_ul, row_lr, CLR_ZERO, CLR_SHADOW, 2 );
  }

  EndClipping( control );

  // HACK! This gets around the issue of how to render headers and scrolls,
  // which do not fall within the client area.
  /*if( !m_header_row->empty() ) {
    Rect header_area(Point(ul.x + static_cast<int>(BORDER_THICK), m_header_row->UpperLeft().y),
                      Point(lr.x - static_cast<int>(BORDER_THICK), m_header_row->LowerRight().y));
    BeginScissorClipping(header_area.ul, header_area.lr);
    GUI::GetGUI()->RenderWindow(m_header_row);
    EndScissorClipping();
  }
  if( m_vscroll ) {
    GUI::GetGUI()->RenderWindow(m_vscroll);
  }
  if( m_hscroll ) {
    GUI::GetGUI()->RenderWindow(m_hscroll);
  }*/

  // ensure that data in occluded cells is not rendered
  bool hide = true;
  for( ListBox::const_iterator it = control.begin(); it != control.end(); ++it ) 
  {
    if( it == control.FirstRowShown() ) 
    {
      hide = false;
    }

    if( hide ) 
    {
      (*it)->Hide();
    }
    else 
    {
      (*it)->Show();
    }

    if( it == last_visible_row ) 
    {
      hide = true;
    }
  } 
}

void 
WidgetRenderer::RenderSlider( const Slider& control, int32 depth )
{
  const Point   UL          = control.GetUpperLeft();
  const Point   LR          = control.GetLowerRight();
  Colour        colourToUse = control.IsDisabled() ? GetDisabledColour(control.GetColour()) : control.GetColour();
  uint32        tabWidth    = control.GetTabWidth();
  uint32        lineWidth   = control.GetLineWidth();
  Point ul, lr;

  //horizontal only
  ul.x = UL.x + tabWidth / 2;
  lr.x = LR.x - tabWidth / 2;
  ul.y = ((LR.y + UL.y) - static_cast<int>(lineWidth)) / 2;
  lr.y = ul.y + static_cast<int>(lineWidth);

  switch( control.GetLineStyle() ) 
  {
  case FLAT:
    FlatRectangle( ul, lr, colourToUse, CLR_BLACK, 1 );
    break;
  case RAISED:
    BeveledRectangle( ul, lr, colourToUse, colourToUse, true, control.GetLineWidth() / 2 );
    break;
  case GROOVED:
    BeveledRectangle( ul, lr, colourToUse, colourToUse, false, control.GetLineWidth() / 2 );
    break;
  }
}