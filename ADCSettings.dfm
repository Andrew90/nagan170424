object ADCSettForm: TADCSettForm
  Left = 177
  Top = 186
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1040#1062#1055
  ClientHeight = 669
  ClientWidth = 741
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 737
    Height = 593
    ActivePage = OtherPage
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnChange = PageControlChange
    object ppPage: TTabSheet
      Caption = #1055#1086#1087#1077#1088#1077#1095#1085#1099#1081' '#1082#1086#1085#1090#1088#1086#1083#1100
      object Label1: TLabel
        Left = 350
        Top = 29
        Width = 166
        Height = 19
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1091#1092#1077#1088#1072' '#1040#1062#1055' ('#1084#1073')'
      end
      object Label2: TLabel
        Left = 330
        Top = 94
        Width = 243
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1086#1087#1088#1086#1089#1072' '#1086#1076#1085#1086#1075#1086' '#1082#1072#1085#1072#1083#1072' ('#1082#1043#1094')'
        WordWrap = True
      end
      object Label3: TLabel
        Left = 350
        Top = 157
        Width = 201
        Height = 19
        Alignment = taCenter
        Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1084#1077#1078#1076#1091' '#1082#1072#1076#1088#1072#1084#1080' ('#1084#1089')'
        WordWrap = True
      end
      object gbPP: TGroupBox
        Left = 8
        Top = 8
        Width = 289
        Height = 457
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 0
      end
      object ppKadr: TEdit
        Left = 586
        Top = 154
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 1
        OnKeyPress = ppSizeBufferKeyPress
      end
      object ppRate: TEdit
        Left = 586
        Top = 93
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 2
        OnKeyPress = ppSizeBufferKeyPress
      end
      object ppSizeBuffer: TEdit
        Left = 586
        Top = 26
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 3
        OnKeyPress = ppSizeBufferKeyPress
      end
    end
    object prPage: TTabSheet
      Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1099#1081' '#1082#1086#1085#1090#1088#1086#1083#1100
      ImageIndex = 1
      object LabelKadr: TLabel
        Left = 350
        Top = 157
        Width = 201
        Height = 19
        Alignment = taCenter
        Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1084#1077#1078#1076#1091' '#1082#1072#1076#1088#1072#1084#1080' ('#1084#1089')'
        WordWrap = True
      end
      object LabelRate: TLabel
        Left = 330
        Top = 96
        Width = 243
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1086#1087#1088#1086#1089#1072' '#1086#1076#1085#1086#1075#1086' '#1082#1072#1085#1072#1083#1072' ('#1082#1043#1094')'
        WordWrap = True
      end
      object LabelSizeBuffer: TLabel
        Left = 350
        Top = 29
        Width = 166
        Height = 19
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1091#1092#1077#1088#1072' '#1040#1062#1055' ('#1084#1073')'
      end
      object gbPR: TGroupBox
        Left = 8
        Top = 8
        Width = 289
        Height = 321
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 0
      end
      object prSizeBuffer: TEdit
        Left = 586
        Top = 26
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 1
        OnKeyPress = ppSizeBufferKeyPress
      end
      object prRate: TEdit
        Left = 586
        Top = 93
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 2
        OnKeyPress = ppSizeBufferKeyPress
      end
      object prKadr: TEdit
        Left = 586
        Top = 154
        Width = 136
        Height = 27
        Enabled = False
        TabOrder = 3
        OnKeyPress = ppSizeBufferKeyPress
      end
    end
    object OtherPage: TTabSheet
      Caption = #1055#1088#1086#1095#1080#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
      ImageIndex = 2
      object LabelMag: TLabel
        Left = 298
        Top = 257
        Width = 130
        Height = 21
        Hint = #1058#1086#1082#1086#1074#1099#1081' '#1076#1072#1090#1095#1080#1082' '#1084#1086#1076#1091#1083#1103' '#1088#1072#1079#1084#1072#1075#1085#1080#1095#1080#1074#1072#1085#1080#1103
        Caption = #1044#1072#1090#1095#1080#1082' '#1090#1086#1082#1072' '#1052#1056
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object LabelSG: TLabel
        Left = 298
        Top = 209
        Width = 87
        Height = 21
        Hint = #1044#1072#1090#1095#1080#1082' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
        Caption = #1044#1072#1090#1095#1080#1082' '#1043#1055
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object lWidthMedianFilter: TLabel
        Left = 368
        Top = 496
        Width = 201
        Height = 19
        Caption = #1064#1080#1088#1080#1085#1072' '#1084#1077#1076#1080#1072#1085#1085#1086#1075#1086' '#1092#1080#1083#1100#1090#1088#1072
      end
      object cbComWithASU: TCheckBox
        Left = 368
        Top = 409
        Width = 249
        Height = 17
        Caption = #1054#1073#1084#1077#1085' '#1089' '#1094#1077#1093#1086#1084' '#1087#1086' COM '#1087#1086#1088#1090#1091
        TabOrder = 0
      end
      object rgSolidGroupSource: TRadioGroup
        Left = 20
        Top = 190
        Width = 680
        Height = 184
        Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
        ItemIndex = 1
        Items.Strings = (
          'LCard'
          'COM '#1087#1086#1088#1090)
        TabOrder = 1
      end
      object gbTransport: TGroupBox
        Left = 20
        Top = 380
        Width = 316
        Height = 161
        Caption = #1058#1088#1072#1085#1089#1087#1086#1088#1090#1085#1099#1077' '#1089#1090#1086#1081#1082#1080
        TabOrder = 2
        object Label7: TLabel
          Left = 16
          Top = 30
          Width = 119
          Height = 19
          Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1088#1072#1073#1086#1090#1099':'
        end
        object Label8: TLabel
          Left = 16
          Top = 75
          Width = 130
          Height = 19
          Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1090#1088#1072#1085#1079#1080#1090#1072':'
        end
        object Label9: TLabel
          Left = 16
          Top = 123
          Width = 129
          Height = 19
          Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1074#1086#1079#1074#1088#1072#1090#1072':'
        end
        object cbWorkSpeed: TComboBox
          Left = 156
          Top = 24
          Width = 145
          Height = 27
          TabOrder = 0
        end
        object cbTransitSpeed: TComboBox
          Left = 156
          Top = 72
          Width = 145
          Height = 27
          TabOrder = 1
        end
        object cbReturnSpeed: TComboBox
          Left = 156
          Top = 120
          Width = 145
          Height = 27
          TabOrder = 2
        end
      end
      object gbSolenoids: TGroupBox
        Left = 20
        Top = 14
        Width = 680
        Height = 170
        Caption = #1057#1086#1083#1077#1085#1086#1080#1076#1099
        TabOrder = 3
        object LabelMagnetic: TLabel
          Left = 17
          Top = 130
          Width = 221
          Height = 21
          Caption = #1050#1086#1085#1090#1088#1086#1083#1100' '#1084#1072#1075#1085#1080#1090#1085#1086#1075#1086' '#1087#1086#1083#1103
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object LabelTemp2: TLabel
          Left = 17
          Top = 90
          Width = 224
          Height = 21
          Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1089#1086#1083#1077#1085#1086#1080#1076#1072' '#8470'2'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object LabelTemp1: TLabel
          Left = 17
          Top = 50
          Width = 224
          Height = 21
          Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1089#1086#1083#1077#1085#1086#1080#1076#1072' '#8470'1'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 400
          Top = 25
          Width = 161
          Height = 19
          Caption = #1050#1088#1080#1090#1080#1095#1077#1089#1082#1086#1077' '#1089#1086#1089#1090#1086#1103#1085#1080#1077
        end
        object NumberCh: TLabel
          Left = 280
          Top = 23
          Width = 93
          Height = 19
          Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
        end
        object SpinTemp1: TCSpinEdit
          Left = 278
          Top = 50
          Width = 100
          Height = 27
          Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 0
        end
        object SpinTemp2: TCSpinEdit
          Left = 279
          Top = 86
          Width = 100
          Height = 27
          Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 1
        end
        object SpinMagnetic: TCSpinEdit
          Left = 279
          Top = 124
          Width = 100
          Height = 27
          Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 2
        end
        object ComboBoxMG: TComboBox
          Left = 400
          Top = 124
          Width = 97
          Height = 27
          Style = csDropDownList
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 3
          Text = #1041#1086#1083#1100#1096#1077
          Items.Strings = (
            #1041#1086#1083#1100#1096#1077
            #1052#1077#1085#1100#1096#1077)
        end
        object ComboBoxT2: TComboBox
          Left = 400
          Top = 86
          Width = 97
          Height = 27
          Style = csDropDownList
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 4
          Text = #1041#1086#1083#1100#1096#1077
          Items.Strings = (
            #1041#1086#1083#1100#1096#1077
            #1052#1077#1085#1100#1096#1077)
        end
        object ComboBoxT1: TComboBox
          Left = 400
          Top = 50
          Width = 97
          Height = 27
          Style = csDropDownList
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 5
          Text = #1041#1086#1083#1100#1096#1077
          Items.Strings = (
            #1041#1086#1083#1100#1096#1077
            #1052#1077#1085#1100#1096#1077)
        end
        object EditT1: TEdit
          Left = 525
          Top = 50
          Width = 84
          Height = 27
          Enabled = False
          NumbersOnly = True
          TabOrder = 6
          OnKeyPress = ppSizeBufferKeyPress
        end
        object EditT2: TEdit
          Left = 525
          Top = 86
          Width = 84
          Height = 27
          Enabled = False
          NumbersOnly = True
          TabOrder = 7
          OnKeyPress = ppSizeBufferKeyPress
        end
        object EditMG: TEdit
          Left = 525
          Top = 124
          Width = 84
          Height = 27
          Enabled = False
          NumbersOnly = True
          TabOrder = 8
          OnKeyPress = ppSizeBufferKeyPress
        end
      end
      object SpinCurr: TCSpinEdit
        Left = 497
        Top = 257
        Width = 71
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 4
      end
      object SpinSG: TCSpinEdit
        Left = 497
        Top = 209
        Width = 71
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 5
      end
      object cbMedianFilter: TCheckBox
        Left = 368
        Top = 457
        Width = 193
        Height = 17
        Caption = #1052#1077#1076#1080#1072#1085#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103
        TabOrder = 6
      end
      object ComboBoxWidthMF: TComboBox
        Left = 584
        Top = 493
        Width = 73
        Height = 27
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 7
        Text = '3'
        Items.Strings = (
          '3'
          '5'
          '7')
      end
      object cbCalcSolidGroup: TCheckBox
        Left = 299
        Top = 340
        Width = 247
        Height = 17
        Caption = #1056#1072#1089#1095#1077#1090' '#1089' '#1087#1086#1084#1086#1097#1100#1102' '#1082#1086#1088#1088#1077#1083#1103#1094#1080#1080
        TabOrder = 8
      end
      object spinTemp: TCSpinEdit
        Left = 497
        Top = 290
        Width = 71
        Height = 29
        MaxValue = 31
        TabOrder = 9
      end
      object cbTemp: TCheckBox
        Left = 298
        Top = 302
        Width = 184
        Height = 17
        Caption = #1044#1072#1090#1095#1080#1082' '#1090#1077#1084#1087#1077#1088#1072#1090#1091#1088#1099
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
    end
    object LIRPage: TTabSheet
      Caption = #1051#1048#1056
      ImageIndex = 3
      object Label5: TLabel
        Left = 115
        Top = 18
        Width = 95
        Height = 19
        Caption = #1044#1083#1080#1085#1072' '#1088#1086#1083#1080#1082#1072
      end
      object Label6: TLabel
        Left = 223
        Top = 18
        Width = 42
        Height = 19
        Caption = #1050#1072#1085#1072#1083
      end
      object eLirToCross: TLabeledEdit
        Left = 616
        Top = 32
        Width = 80
        Height = 27
        EditLabel.Width = 241
        EditLabel.Height = 19
        EditLabel.ParentCustomHint = False
        EditLabel.BiDiMode = bdLeftToRight
        EditLabel.Caption = #1056#1072#1089#1089#1090#1086#1103#1085#1080#1077' '#1051#1048#1056' - '#1055#1086#1087#1077#1088#1077#1095#1085#1099#1081', '#1084#1084
        EditLabel.Color = clBtnFace
        EditLabel.Font.Charset = RUSSIAN_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'Times New Roman'
        EditLabel.Font.Style = []
        EditLabel.ParentBiDiMode = False
        EditLabel.ParentColor = False
        EditLabel.ParentFont = False
        EditLabel.ParentShowHint = False
        EditLabel.ShowHint = False
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 0
      end
      object eLirToLinear: TLabeledEdit
        Left = 616
        Top = 81
        Width = 80
        Height = 27
        EditLabel.Width = 241
        EditLabel.Height = 19
        EditLabel.Caption = #1056#1072#1089#1089#1090#1086#1103#1085#1080#1077' '#1051#1048#1056' - '#1055#1088#1086#1076#1086#1083#1100#1085#1099#1081', '#1084#1084
        EditLabel.Color = clBtnFace
        EditLabel.Font.Charset = RUSSIAN_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'Times New Roman'
        EditLabel.Font.Style = []
        EditLabel.ParentColor = False
        EditLabel.ParentFont = False
        EditLabel.Transparent = True
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 1
      end
      object eLirToThickness: TLabeledEdit
        Left = 616
        Top = 130
        Width = 80
        Height = 27
        EditLabel.Width = 241
        EditLabel.Height = 19
        EditLabel.Caption = #1056#1072#1089#1089#1090#1086#1103#1085#1080#1077' '#1051#1048#1056' - '#1058#1086#1083#1097#1080#1085#1086#1084#1077#1088', '#1084#1084
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 2
      end
      object eLirToMarker: TLabeledEdit
        Left = 616
        Top = 179
        Width = 80
        Height = 27
        EditLabel.Width = 273
        EditLabel.Height = 19
        EditLabel.Caption = #1056#1072#1089#1089#1090#1086#1103#1085#1080#1077' '#1051#1048#1056' - '#1044#1077#1092#1077#1082#1090#1086#1086#1090#1084#1077#1090#1095#1080#1082', '#1084#1084
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 3
      end
      object eImpulsePerRound: TLabeledEdit
        Left = 616
        Top = 228
        Width = 80
        Height = 27
        EditLabel.Width = 190
        EditLabel.Height = 19
        EditLabel.Caption = #1063#1080#1089#1083#1086' '#1080#1084#1087#1091#1083#1100#1089#1086#1074' '#1085#1072' '#1086#1073#1086#1088#1086#1090
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 4
      end
      object eMarkerTime: TLabeledEdit
        Left = 616
        Top = 277
        Width = 80
        Height = 27
        EditLabel.Width = 293
        EditLabel.Height = 19
        EditLabel.Caption = #1042#1088#1077#1084#1103' '#1089#1088#1072#1073#1072#1090#1099#1074#1072#1085#1080#1103' '#1076#1077#1092#1077#1082#1090#1086#1086#1090#1084#1077#1090#1095#1080#1082#1072', '#1084#1089
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 5
      end
      object ePauseMixing: TLabeledEdit
        Left = 616
        Top = 376
        Width = 80
        Height = 27
        EditLabel.Width = 256
        EditLabel.Height = 19
        EditLabel.Caption = #1055#1072#1091#1079#1099' '#1084#1077#1078#1076#1091' '#1087#1077#1088#1077#1084#1077#1096#1080#1074#1072#1085#1080#1103#1084#1080', '#1084#1080#1085
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 6
      end
      object eMixingTime: TLabeledEdit
        Left = 616
        Top = 326
        Width = 80
        Height = 27
        EditLabel.Width = 218
        EditLabel.Height = 19
        EditLabel.Caption = #1042#1088#1077#1084#1103' '#1087#1077#1088#1077#1084#1077#1096#1080#1074#1072#1085#1080#1103' '#1082#1088#1072#1089#1082#1080', '#1089
        LabelPosition = lpLeft
        LabelSpacing = 5
        NumbersOnly = True
        TabOrder = 7
      end
      object cbCalibrate: TCheckBox
        Left = 48
        Top = 184
        Width = 161
        Height = 17
        Caption = #1050#1072#1083#1080#1073#1088#1086#1074#1082#1072' '#1051#1048#1056#1086#1074
        TabOrder = 8
      end
    end
  end
  object bSave: TButton
    Left = 241
    Top = 610
    Width = 113
    Height = 41
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = bSaveClick
  end
  object bExit: TButton
    Left = 380
    Top = 610
    Width = 113
    Height = 41
    Caption = #1042#1099#1081#1090#1080
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = bExitClick
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 552
    Top = 8
  end
end
