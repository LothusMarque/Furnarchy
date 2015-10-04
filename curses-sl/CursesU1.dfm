object CursesMain: TCursesMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Curses'
  ClientHeight = 480
  ClientWidth = 640
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  ShowHint = True
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainBackground: TImage
    Left = 0
    Top = 0
    Width = 640
    Height = 461
    Align = alClient
    Center = True
    ExplicitLeft = 398
    ExplicitTop = 272
    ExplicitWidth = 105
    ExplicitHeight = 105
  end
  object LogoImg: TImage
    Left = 393
    Top = 293
    Width = 244
    Height = 167
    Center = True
    Proportional = True
    Stretch = True
  end
  object LogoImg2: TImage
    Left = 330
    Top = 102
    Width = 75
    Height = 159
    Center = True
    OnClick = LogoImg2Click
  end
  object PopMsg1: TImage
    Left = 447
    Top = 353
    Width = 120
    Height = 78
    Center = True
    Proportional = True
    Stretch = True
    Transparent = True
  end
  object FileScanner: TFileListBox
    Left = 0
    Top = 153
    Width = 145
    Height = 97
    ItemHeight = 13
    Mask = '*.ini'
    TabOrder = 3
    Visible = False
  end
  object TabSet: TPageControl
    Left = 8
    Top = 264
    Width = 383
    Height = 196
    ActivePage = SettingsTab
    TabOrder = 0
    object DescTab: TTabSheet
      Caption = 'Description'
      object DescLengthLabel: TLabel
        Left = 0
        Top = 155
        Width = 375
        Height = 13
        Align = alBottom
        ExplicitWidth = 3
      end
      object DescMemo: TMemo
        Left = 0
        Top = 0
        Width = 375
        Height = 155
        Align = alClient
        MaxLength = 1000
        ScrollBars = ssVertical
        TabOrder = 0
        OnChange = DescMemoChange
      end
    end
    object AFKDescTab: TTabSheet
      Caption = 'AFK Description'
      ImageIndex = 1
      object AFKDescLengthLabel: TLabel
        Left = 0
        Top = 155
        Width = 375
        Height = 13
        Align = alBottom
        ExplicitWidth = 3
      end
      object AFKDescMemo: TMemo
        Left = 0
        Top = 0
        Width = 238
        Height = 155
        Align = alClient
        MaxLength = 1000
        ScrollBars = ssVertical
        TabOrder = 0
        OnChange = AFKDescMemoChange
      end
      object GroupBox3: TGroupBox
        Left = 238
        Top = 0
        Width = 137
        Height = 155
        Align = alRight
        Caption = 'Tag Help'
        TabOrder = 1
        object HelpMemo1: TMemo
          Left = 2
          Top = 15
          Width = 133
          Height = 138
          Align = alClient
          Lines.Strings = (
            'HelpMemo1')
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
    end
    object AfkWhisperTab: TTabSheet
      Caption = 'Away Whisper'
      ImageIndex = 2
      object AwayWhisperLabel: TLabel
        Left = 0
        Top = 155
        Width = 375
        Height = 13
        Align = alBottom
        ExplicitWidth = 3
      end
      object AwayWhisperMemo: TMemo
        Left = 0
        Top = 0
        Width = 238
        Height = 155
        Align = alClient
        ScrollBars = ssVertical
        TabOrder = 0
        OnChange = AwayWhisperMemoChange
      end
      object GroupBox4: TGroupBox
        Left = 238
        Top = 0
        Width = 137
        Height = 155
        Align = alRight
        Caption = 'Tag Help'
        TabOrder = 1
        object HelpMemo2: TMemo
          Left = 2
          Top = 15
          Width = 133
          Height = 138
          Align = alClient
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
    end
    object AutoResponse: TTabSheet
      Caption = 'AutoResponse'
      ImageIndex = 4
      object AutoResLabel: TLabel
        Left = 0
        Top = 155
        Width = 375
        Height = 13
        Align = alBottom
        ExplicitWidth = 3
      end
      object AutoResMemo: TMemo
        Left = 0
        Top = 0
        Width = 238
        Height = 155
        Align = alClient
        ScrollBars = ssVertical
        TabOrder = 0
        OnChange = AutoResMemoChange
      end
      object GroupBox6: TGroupBox
        Left = 238
        Top = 0
        Width = 137
        Height = 155
        Align = alRight
        Caption = 'Tag Help'
        TabOrder = 1
        object HelpMemo3: TMemo
          Left = 2
          Top = 15
          Width = 133
          Height = 138
          Align = alClient
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
    end
    object SettingsTab: TTabSheet
      Caption = 'Settings'
      ImageIndex = 3
      object SettingsSB: TScrollBox
        Left = 0
        Top = 0
        Width = 375
        Height = 168
        VertScrollBar.Position = 5
        VertScrollBar.Tracking = True
        Align = alClient
        TabOrder = 0
        object Label5: TLabel
          Left = 3
          Top = 2
          Width = 126
          Height = 13
          Caption = 'Activate Away Mode after'
        end
        object Label10: TLabel
          Left = 181
          Top = 2
          Width = 170
          Height = 13
          Caption = 'minutes of inactivity (Zero disables)'
        end
        object Label7: TLabel
          Left = 2
          Top = 27
          Width = 214
          Height = 13
          Caption = 'Automatically disconnect from Furcadia after'
        end
        object Label8: TLabel
          Left = 269
          Top = 27
          Width = 48
          Height = 13
          Caption = 'minutes in'
        end
        object Label9: TLabel
          Left = 12
          Top = 46
          Width = 134
          Height = 13
          Caption = 'Away Mode. (Zero disables)'
        end
        object Label11: TLabel
          Left = 3
          Top = 69
          Width = 162
          Height = 13
          Caption = 'Activate Portrat # (Zero disables)'
        end
        object Label12: TLabel
          Left = 62
          Top = 88
          Width = 143
          Height = 13
          Caption = 'when Away Mode is activated'
        end
        object Label13: TLabel
          Left = 62
          Top = 116
          Width = 157
          Height = 13
          Caption = 'when returning from Away Mode'
        end
        object AFKTimerSpin: TSpinEdit
          Left = 132
          Top = -1
          Width = 47
          Height = 22
          MaxValue = 0
          MinValue = 0
          TabOrder = 0
          Value = 0
          OnChange = AFKTimerSpinChange
        end
        object DisConnSpin: TSpinEdit
          Left = 217
          Top = 23
          Width = 47
          Height = 22
          MaxValue = 0
          MinValue = 0
          TabOrder = 1
          Value = 0
          OnChange = AFKTimerSpinChange
        end
        object AwayPortSpin: TSpinEdit
          Left = 12
          Top = 85
          Width = 47
          Height = 22
          MaxValue = 0
          MinValue = 0
          TabOrder = 2
          Value = 0
          OnChange = AFKTimerSpinChange
        end
        object ReturnPortSpin: TSpinEdit
          Left = 12
          Top = 113
          Width = 47
          Height = 22
          MaxValue = 0
          MinValue = 0
          TabOrder = 3
          Value = 0
          OnChange = AFKTimerSpinChange
        end
        object AutoResCB: TCheckBox
          Left = 3
          Top = 141
          Width = 344
          Height = 23
          Caption = 
            'Send automatic response to all furres who whisper you but who ar' +
            'e not on your online list'
          TabOrder = 4
          WordWrap = True
          OnClick = AutoResCBClick
        end
      end
    end
  end
  object AvSettings: TGroupBox
    Left = 169
    Top = 11
    Width = 158
    Height = 205
    Caption = 'Avatar Settings'
    TabOrder = 1
    object Label3: TLabel
      Left = 7
      Top = 88
      Width = 70
      Height = 13
      Caption = 'Color Segment'
    end
    object Label4: TLabel
      Left = 7
      Top = 124
      Width = 25
      Height = 13
      Caption = 'Color'
    end
    object Label1: TLabel
      Left = 7
      Top = 16
      Width = 36
      Height = 13
      Caption = 'Species'
    end
    object Label2: TLabel
      Left = 7
      Top = 52
      Width = 35
      Height = 13
      Caption = 'Gender'
    end
    object CB_Color: TComboBox
      Left = 7
      Top = 137
      Width = 145
      Height = 19
      Style = csOwnerDrawFixed
      ItemHeight = 13
      TabOrder = 0
      OnChange = CB_ColorChange
      OnDrawItem = CB_ColorDrawItem
    end
    object CB_ColorSeg: TComboBox
      Left = 7
      Top = 101
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = 'Fur'
      OnChange = CB_ColorSegChange
      Items.Strings = (
        'Fur'
        'Markings'
        'Hair'
        'Eyes'
        'Badge'
        'Vest'
        'Bracers'
        'Cape'
        'Boots'
        'Trousers')
    end
    object CB_Gender: TComboBox
      Left = 7
      Top = 65
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 2
      Text = 'Female'
      OnChange = CB_GenderChange
      Items.Strings = (
        'Female'
        'Male'
        'Unspecified')
    end
    object CB_Species: TComboBox
      Left = 7
      Top = 29
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 3
      Text = 'Rodent'
      OnChange = CB_SpeciesChange
      Items.Strings = (
        'Rodent'
        'Equine'
        'Feline'
        'Canine'
        'Musteline'
        'Lapine'
        'Squirrel'
        'Bovine'
        'Ursine'
        'Bugge')
    end
    object CCEdit: TLabeledEdit
      Left = 7
      Top = 176
      Width = 128
      Height = 21
      EditLabel.Width = 48
      EditLabel.Height = 13
      EditLabel.Caption = 'Colorcode'
      TabOrder = 4
      OnChange = CCEditChange
    end
    object RandClothesBtn: TButton
      Left = 136
      Top = 176
      Width = 19
      Height = 21
      Hint = 'Randomize (Right-click for options)'
      Caption = 'R'
      PopupMenu = RandomTypeMenu
      TabOrder = 5
      OnClick = RandClothesBtnClick
    end
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 11
    Width = 159
    Height = 205
    Caption = 'Characters'
    TabOrder = 2
    object CharacterList: TListBox
      Left = 2
      Top = 15
      Width = 155
      Height = 186
      Align = alClient
      IntegralHeight = True
      ItemHeight = 13
      PopupMenu = CharListPopup
      Sorted = True
      TabOrder = 0
      OnClick = CharacterListClick
      OnDblClick = PlayBtnClick
      OnKeyDown = CharacterListKeyDown
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 461
    Width = 640
    Height = 19
    Panels = <
      item
        Width = 100
      end>
  end
  object GroupBox5: TGroupBox
    Left = 333
    Top = 11
    Width = 297
    Height = 86
    TabOrder = 5
    object NameEdit: TLabeledEdit
      Left = 6
      Top = 23
      Width = 136
      Height = 21
      EditLabel.Width = 27
      EditLabel.Height = 13
      EditLabel.Caption = 'Name'
      TabOrder = 0
    end
    object PassEdit: TLabeledEdit
      Left = 148
      Top = 23
      Width = 141
      Height = 21
      EditLabel.Width = 46
      EditLabel.Height = 13
      EditLabel.Caption = 'Password'
      PasswordChar = '*'
      TabOrder = 1
    end
    object SaveBtn: TButton
      Left = 6
      Top = 52
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 2
      OnClick = SaveBtnClick
    end
    object SaveAsBtn: TButton
      Left = 87
      Top = 52
      Width = 75
      Height = 25
      Caption = 'Save As'
      TabOrder = 3
      OnClick = SaveAsBtnClick
    end
    object PlayBtn: TButton
      Left = 194
      Top = 50
      Width = 79
      Height = 25
      Caption = 'Play!'
      TabOrder = 4
      OnClick = PlayBtnClick
    end
    object Button1: TButton
      Left = 274
      Top = 50
      Width = 15
      Height = 25
      Caption = 'V'
      TabOrder = 5
      OnClick = Button1Click
    end
  end
  object INISettingsBtn: TButton
    Left = 4
    Top = 219
    Width = 159
    Height = 39
    Caption = 'Curses Configuration'
    TabOrder = 6
    OnClick = INISettingsBtnClick
  end
  object GroupBox2: TGroupBox
    Left = 407
    Top = 112
    Width = 225
    Height = 179
    Caption = 'Preview'
    TabOrder = 7
    object Label6: TLabel
      Left = 14
      Top = 157
      Width = 21
      Height = 13
      Caption = 'Digo'
    end
    object PortPrevPanel: TPanel
      Left = 3
      Top = 13
      Width = 101
      Height = 104
      BevelOuter = bvLowered
      ParentColor = True
      TabOrder = 0
      object PortDisplay: TImage
        Left = 3
        Top = 5
        Width = 95
        Height = 95
        Center = True
        PopupMenu = CustPortMenu
      end
    end
    object PrevAvaPanel: TPanel
      Left = 105
      Top = 13
      Width = 117
      Height = 122
      BevelOuter = bvLowered
      ParentColor = True
      TabOrder = 1
      object AvaDisplay: TImage
        Left = 2
        Top = 5
        Width = 113
        Height = 113
        Center = True
        Transparent = True
      end
    end
    object DigoCB: TComboBox
      Left = 45
      Top = 154
      Width = 165
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 2
      OnChange = DigoCBChange
    end
    object AvaPrevBtn: TButton
      Left = 104
      Top = 137
      Width = 31
      Height = 14
      Caption = '<'
      TabOrder = 3
      OnClick = AvaPrevBtnClick
    end
    object AvaNextBtn: TButton
      Left = 138
      Top = 137
      Width = 29
      Height = 14
      Caption = '>'
      TabOrder = 4
      OnClick = AvaNextBtnClick
    end
    object Button2: TButton
      Left = 3
      Top = 119
      Width = 98
      Height = 20
      Caption = 'Port Settings'
      TabOrder = 5
      OnClick = Button2Click
      OnMouseUp = Button2MouseUp
    end
  end
  object WOFurnarchy: TCheckBox
    Left = 523
    Top = 98
    Width = 107
    Height = 17
    Caption = 'Without Furnarchy'
    TabOrder = 8
    OnClick = WOFurnarchyClick
  end
  object FurnConfigBtn: TButton
    Left = 169
    Top = 219
    Width = 158
    Height = 39
    Caption = 'Furnarchy Configuration'
    TabOrder = 9
    OnClick = FurnConfigBtnClick
  end
  object XPManifest1: TXPManifest
    Left = 422
    Top = 412
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'ini'
    Filter = 'Character INI File|*.ini|All Files|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 410
    Top = 302
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Filter = 'Furcadia Files|*.fox;*.fsh|FOX|*.fox|FSH|*.fsh|All Files|*.*'
    Left = 414
    Top = 358
  end
  object CustPortMenu: TPopupMenu
    Left = 574
    Top = 344
    object ShowCustomPort1: TMenuItem
      AutoCheck = True
      Caption = 'Show Custom Port'
      Checked = True
      OnClick = ShowCustomPort1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object ChooseCustomPort1: TMenuItem
      Caption = 'Choose Custom Port...'
      OnClick = ChooseCustomPort1Click
    end
    object ClearCustomPort1: TMenuItem
      Caption = 'Clear Custom Port'
      OnClick = ClearCustomPort1Click
    end
  end
  object CharListPopup: TPopupMenu
    Left = 574
    Top = 300
    object Setasdefault1: TMenuItem
      Caption = 'Set as default...'
      OnClick = SetDefINIBtnClick
    end
    object N5: TMenuItem
      Caption = '-'
    end
    object Delete1: TMenuItem
      Caption = 'Delete...'
      OnClick = Delete1Click
    end
  end
  object MainMenu1: TMainMenu
    Left = 296
    Top = 4
    object File1: TMenuItem
      Caption = 'File'
      object Play1: TMenuItem
        Caption = 'Play!'
        OnClick = PlayBtnClick
      end
      object RunCharacterCreation1: TMenuItem
        Caption = 'Run Character Creation'
        OnClick = RunCharacterCreation2Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Save1: TMenuItem
        Caption = 'Save'
        OnClick = SaveBtnClick
      end
      object SaveAs1: TMenuItem
        Caption = 'Save As...'
        OnClick = SaveAsBtnClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Options1: TMenuItem
      Caption = 'Options'
      object WithoutFurnarchy1: TMenuItem
        AutoCheck = True
        Caption = 'Without Furnarchy'
        OnClick = WOFurnarchyClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object CursesConfiguration1: TMenuItem
        Caption = 'Curses Configuration...'
        OnClick = INISettingsBtnClick
      end
      object FurnarchyConfiguration1: TMenuItem
        Caption = 'Furnarchy Configuration...'
        OnClick = FurnConfigBtnClick
      end
    end
  end
  object DropPopMenu: TPopupMenu
    Left = 466
    Top = 96
    object RunCharacterCreation2: TMenuItem
      Caption = 'Run Character Creation'
      OnClick = RunCharacterCreation2Click
    end
  end
  object RandomTypeMenu: TPopupMenu
    Left = 574
    Top = 390
    object RandomClothes1: TMenuItem
      Caption = 'Random Clothes'
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = RandomClothes1Click
    end
    object RandomColors1: TMenuItem
      Caption = 'Random Colors'
      GroupIndex = 1
      RadioItem = True
      OnClick = RandomClothes1Click
    end
    object RandomEverything1: TMenuItem
      Caption = 'Random Everything'
      GroupIndex = 1
      RadioItem = True
      OnClick = RandomClothes1Click
    end
  end
  object DigoDropPop: TPopupMenu
    Left = 500
    Top = 300
    object SavedigoselectionThisINI1: TMenuItem
      AutoCheck = True
      Caption = 'Save digo selection (This INI)'
      Enabled = False
    end
    object N6: TMenuItem
      Caption = '-'
    end
    object ClearALLDigoselections1: TMenuItem
      Caption = 'Clear ALL Digo selections'
      OnClick = ClearALLDigoselections1Click
    end
  end
end
