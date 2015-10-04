object TwitCfgFrm: TTwitCfgFrm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Configuration'
  ClientHeight = 220
  ClientWidth = 327
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 163
    Top = 192
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 247
    Top = 192
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 20
    Width = 137
    Height = 112
    Caption = 'Account Information'
    TabOrder = 2
    object AccountEdit: TLabeledEdit
      Left = 6
      Top = 31
      Width = 121
      Height = 21
      EditLabel.Width = 48
      EditLabel.Height = 13
      EditLabel.Caption = 'Username'
      TabOrder = 0
    end
    object PassEdit: TLabeledEdit
      Left = 6
      Top = 72
      Width = 121
      Height = 21
      EditLabel.Width = 46
      EditLabel.Height = 13
      EditLabel.Caption = 'Password'
      PasswordChar = '*'
      TabOrder = 1
      Text = 'kqB9PxtfemnPbK'
    end
  end
  object GroupBox2: TGroupBox
    Left = 138
    Top = 20
    Width = 187
    Height = 112
    Caption = 'Settings'
    TabOrder = 3
    object Label1: TLabel
      Left = 29
      Top = 16
      Width = 120
      Height = 13
      Caption = 'Minutes between refresh'
    end
    object CheckTimerEdit: TSpinEdit
      Left = 29
      Top = 31
      Width = 121
      Height = 22
      MaxValue = 30
      MinValue = 3
      TabOrder = 0
      Value = 5
    end
    object RefreshChk: TCheckBox
      Left = 29
      Top = 56
      Width = 81
      Height = 17
      Caption = 'Auto-refresh'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object StartChk: TCheckBox
      Left = 29
      Top = 73
      Width = 119
      Height = 17
      Caption = 'Check on Startup'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object BufDisplayChk: TCheckBox
      Left = 29
      Top = 90
      Width = 116
      Height = 17
      Caption = 'Chat Buffer Display'
      TabOrder = 3
    end
  end
  object CredPanel: TPanel
    Left = 0
    Top = 0
    Width = 327
    Height = 21
    Align = alTop
    Caption = '(Credits)'
    TabOrder = 4
    OnClick = CredPanelClick
  end
  object GroupBox3: TGroupBox
    Left = 0
    Top = 132
    Width = 325
    Height = 58
    Caption = 'Sounds'
    TabOrder = 5
    object Label2: TLabel
      Left = 6
      Top = 16
      Width = 54
      Height = 13
      Caption = 'New Tweet'
    end
    object NewTweetEdit: TEdit
      Left = 24
      Top = 31
      Width = 170
      Height = 21
      TabOrder = 0
      Text = 'NewTweetEdit'
    end
    object NewTweetSndChk: TCheckBox
      Left = 6
      Top = 33
      Width = 19
      Height = 17
      TabOrder = 1
    end
    object NewTweetBrseBtn: TButton
      Left = 219
      Top = 31
      Width = 59
      Height = 20
      Caption = 'Browse...'
      TabOrder = 2
    end
    object NewTweetDefBtn: TButton
      Left = 278
      Top = 31
      Width = 43
      Height = 20
      Caption = 'Default'
      TabOrder = 3
    end
    object Button1: TButton
      Left = 196
      Top = 31
      Width = 24
      Height = 20
      Caption = '>'
      TabOrder = 4
      OnClick = Button1Click
    end
  end
  object Button3: TButton
    Left = 2
    Top = 192
    Width = 99
    Height = 25
    Caption = 'Recommendations'
    TabOrder = 7
    OnClick = Button3Click
  end
  object RecsPanel: TPanel
    Left = 328
    Top = 20
    Width = 327
    Height = 196
    BevelEdges = [beLeft, beRight, beBottom]
    BevelOuter = bvNone
    TabOrder = 6
    object Label3: TLabel
      Left = 8
      Top = 9
      Width = 157
      Height = 13
      Caption = 'Twitter accounts of interest'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 28
      Width = 49
      Height = 13
      Caption = 'Furnarchy'
    end
    object Label5: TLabel
      Left = 8
      Top = 56
      Width = 86
      Height = 13
      Caption = 'Furcadia Art Zone'
    end
    object Label6: TLabel
      Left = 8
      Top = 84
      Width = 93
      Height = 13
      Caption = 'Furcadia Alt Market'
    end
    object Label7: TLabel
      Left = 8
      Top = 112
      Width = 43
      Height = 13
      Caption = 'Damadar'
    end
    object Label8: TLabel
      Left = 8
      Top = 139
      Width = 71
      Height = 13
      Caption = 'Lothus Marque'
    end
    object Label9: TLabel
      Left = 14
      Top = 41
      Width = 138
      Height = 13
      Caption = 'http://twitter.com/furnarchy'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
      OnClick = Label13Click
    end
    object Label10: TLabel
      Left = 14
      Top = 69
      Width = 171
      Height = 13
      Caption = 'http://twitter.com/FurcadiaArtZone'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
      OnClick = Label13Click
    end
    object Label11: TLabel
      Left = 14
      Top = 97
      Width = 158
      Height = 13
      Caption = 'http://twitter.com/FurcAltMarket'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
      OnClick = Label13Click
    end
    object Label12: TLabel
      Left = 14
      Top = 124
      Width = 133
      Height = 13
      Caption = 'http://twitter.com/damadar'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
      OnClick = Label13Click
    end
    object Label13: TLabel
      Left = 14
      Top = 152
      Width = 156
      Height = 13
      Caption = 'http://twitter.com/lothusmarque'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
      OnClick = Label13Click
    end
    object Button2: TButton
      Left = 128
      Top = 176
      Width = 75
      Height = 18
      Caption = 'Ok'
      TabOrder = 0
      OnClick = Button2Click
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'WAVE Sound (*.wav)|*.wav'
    Left = 276
    Top = 186
  end
  object CreditsTimer: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = CreditsTimerTimer
    Left = 278
    Top = 190
  end
end
