object CFGForm: TCFGForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 256
  ClientWidth = 354
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 354
    Height = 165
    Align = alTop
    Caption = 'Ini File Paths'
    TabOrder = 0
    object Label1: TLabel
      Left = 2
      Top = 15
      Width = 350
      Height = 26
      Align = alTop
      Caption = 
        'These folders will be searched for character INI files. Right cl' +
        'ick on the list to modify it.'
      WordWrap = True
      ExplicitWidth = 341
    end
    object PathListBox: TListBox
      Left = 2
      Top = 41
      Width = 350
      Height = 122
      Align = alClient
      ItemHeight = 13
      PopupMenu = PopupMenu1
      TabOrder = 0
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 224
    Width = 354
    Height = 32
    Align = alBottom
    BevelEdges = [beLeft, beRight, beBottom]
    BevelInner = bvRaised
    BevelKind = bkFlat
    BevelOuter = bvNone
    DragKind = dkDock
    TabOrder = 1
    object Button1: TButton
      Left = 100
      Top = 2
      Width = 75
      Height = 25
      Caption = 'Ok'
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 178
      Top = 2
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = Button2Click
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 193
    Width = 354
    Height = 31
    Align = alBottom
    TabOrder = 2
    object Label2: TLabel
      Left = 1
      Top = 1
      Width = 352
      Height = 29
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Curses Standalone V '#169' Mercenary Enclave Productions && Entwined ' +
        'Studios'
      WordWrap = True
      ExplicitLeft = 2
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 165
    Width = 354
    Height = 28
    Align = alClient
    TabOrder = 3
    object WinPosSaveChk: TCheckBox
      Left = 6
      Top = 6
      Width = 159
      Height = 17
      Caption = 'Restore last window position'
      TabOrder = 0
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 194
    Top = 76
    object AddFolder1: TMenuItem
      Caption = 'Add Folder...'
      OnClick = AddFolder1Click
    end
    object Remove1: TMenuItem
      Caption = 'Remove'
      OnClick = Remove1Click
    end
  end
end
