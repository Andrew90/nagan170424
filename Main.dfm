object MainForm: TMainForm
  Left = -44
  Top = 0
  Caption = #1052#1086#1076#1091#1083#1100' '#1101#1083#1077#1082#1090#1088#1086#1084#1072#1075#1085#1080#1090#1085#1086#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1080
  ClientHeight = 836
  ClientWidth = 1284
  Color = clBtnFace
  Constraints.MinHeight = 840
  Constraints.MinWidth = 1180
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  WindowState = wsMaximized
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnMouseDown = FormMouseDown
  OnResize = MainFormInit
  PixelsPerInch = 96
  TextHeight = 13
  object bTest: TButton
    Left = 26
    Top = 55
    Width = 119
    Height = 41
    Caption = 'F2 '#1058#1077#1089#1090
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    Visible = False
    OnClick = bTestClick
    OnKeyPress = FormKeyPress
  end
  object bView: TButton
    Left = 52
    Top = 55
    Width = 119
    Height = 41
    Caption = 'F3 '#1055#1088#1086#1089#1084#1086#1090#1088
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = bViewClick
    OnKeyPress = FormKeyPress
  end
  object bWork: TButton
    Left = 179
    Top = 55
    Width = 140
    Height = 41
    Caption = 'F5 '#1056#1072#1073#1086#1090#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = bWorkClick
    OnKeyPress = FormKeyPress
  end
  object gbCrossDef: TGroupBox
    Left = 0
    Top = 102
    Width = 1353
    Height = 122
    Caption = #1055#1086#1087#1077#1088#1077#1095#1085#1072#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1103
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    object Label1: TLabel
      Left = 261
      Top = 16
      Width = 98
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1073#1088#1072#1082#1072':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 474
      Top = 16
      Width = 117
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 824
      Top = 16
      Width = 119
      Height = 19
      Caption = #1052#1105#1088#1090#1074#1072#1103' '#1079#1086#1085#1072' 1:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Left = 1085
      Top = 16
      Width = 119
      Height = 19
      Caption = #1052#1105#1088#1090#1074#1072#1103' '#1079#1086#1085#1072' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object CrossDefectChart: TChart
      Left = 2
      Top = 56
      Width = 1349
      Height = 64
      BackWall.Transparent = False
      BackWall.Shadow.Visible = True
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 0
      MarginRight = 0
      MarginTop = 3
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Maximum = 10.000000000000000000
      BottomAxis.EndPosition = 99.000000000000000000
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 4.159999999999990000
      DepthAxis.Minimum = 3.159999999999990000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 4.159999999999990000
      DepthTopAxis.Minimum = 3.159999999999990000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Maximum = 100.000000000000000000
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Visible = False
      View3D = False
      Align = alBottom
      BevelOuter = bvNone
      BevelWidth = 2
      TabOrder = 0
      OnClick = ChartClick
      PrintMargins = (
        15
        47
        15
        47)
      ColorPaletteIndex = 13
      object Series1: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series12: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        Title = 'Series2'
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series13: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        Title = 'Series3'
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series14: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        Title = 'Series4'
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series5: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series6: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series7: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series8: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series9: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series10: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series11: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
      object Series15: TBarSeries
        BarBrush.Color = 16384
        BarBrush.Gradient.EndColor = clLime
        ColorEachPoint = True
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Visible = False
        ShowInLegend = False
        Title = 'Series12'
        BarWidthPercent = 100
        Emboss.Color = 8947848
        Gradient.EndColor = clLime
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8947848
        UseYOrigin = False
        YOrigin = 1.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          02070000000000000000003440FF0000000000000000003440FFFF0000000000
          000000344000FF00000000000000003440FF000000000000000000344000FF00
          00000000000000344000FF0000000000000000344000000020}
      end
    end
    object eCrossBorder1: TEdit
      Left = 365
      Top = 13
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1086#1090#1073#1088#1072#1082#1086#1074#1082#1080' '#1074' '#1087#1088#1086#1094#1077#1085#1090#1072#1093' '#1086#1090' '#1091#1088#1086#1074#1085#1103' '#1089#1080#1075#1085#1072#1083#1072
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object eCrossBorder2: TEdit
      Left = 597
      Top = 13
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2. '#1044#1086#1083#1078#1077#1085' '#1073#1099#1090#1100' '#1084#1077#1085#1100#1096#1077' '#1087#1086#1088#1086#1075#1072' '#1073#1088#1072#1082#1072
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object eCrossDeadZoneStart: TEdit
      Left = 949
      Top = 13
      Width = 73
      Height = 27
      Hint = #1053#1077#1082#1086#1085#1090#1088#1086#1083#1080#1088#1091#1077#1084#1072#1103' '#1079#1086#1085#1072' '#1074' '#1085#1072#1095#1072#1083#1077' '#1090#1088#1091#1073#1099', '#1074' '#1084#1084
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object eCrossDeadZoneFinish: TEdit
      Left = 1210
      Top = 13
      Width = 73
      Height = 27
      Hint = #1053#1077#1082#1086#1085#1090#1088#1086#1083#1080#1088#1091#1077#1084#1072#1103' '#1079#1086#1085#1072' '#1074' '#1082#1086#1085#1094#1077' '#1090#1088#1091#1073#1099', '#1074' '#1084#1084
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
  end
  object gbLinearDef: TGroupBox
    Left = 8
    Top = 230
    Width = 1353
    Height = 123
    Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1072#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1103
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentColor = False
    ParentFont = False
    TabOrder = 12
    object Label13: TLabel
      Left = 1077
      Top = 15
      Width = 119
      Height = 19
      Caption = #1052#1105#1088#1090#1074#1072#1103' '#1079#1086#1085#1072' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label14: TLabel
      Left = 253
      Top = 15
      Width = 98
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1073#1088#1072#1082#1072':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 466
      Top = 15
      Width = 117
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 816
      Top = 13
      Width = 119
      Height = 19
      Caption = #1052#1105#1088#1090#1074#1072#1103' '#1079#1086#1085#1072' 1:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object LinearDefectChart: TChart
      Left = 2
      Top = 40
      Width = 1349
      Height = 81
      BackWall.Transparent = False
      BackWall.Shadow.Visible = True
      BottomWall.Transparent = True
      Legend.CurrentPage = False
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 0
      MarginRight = 0
      MarginTop = 3
      RightWall.Transparent = True
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Maximum = 10.000000000000000000
      BottomAxis.EndPosition = 99.000000000000000000
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 2.030000000000001000
      DepthAxis.Minimum = 1.030000000000001000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 2.030000000000001000
      DepthTopAxis.Minimum = 1.030000000000001000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Maximum = 253.000000000000000000
      LeftAxis.Minimum = 153.000000000000000000
      Pages.ScaleLastPage = False
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      RightAxis.Visible = False
      TopAxis.Visible = False
      View3D = False
      Zoom.Brush.Style = bsSolid
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      OnClick = ChartClick
      PrintMargins = (
        15
        47
        15
        47)
      ColorPaletteIndex = 13
      object Series2: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8684676
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8684676
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series16: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8618883
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8618883
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series17: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8618883
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8618883
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series18: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8618883
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8618883
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series19: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8553090
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8553090
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series20: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8553090
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8553090
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series21: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8618883
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8618883
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object Series22: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        BarWidthPercent = 100
        Emboss.Color = 8618883
        MultiBar = mbStacked100
        OffsetPercent = 50
        Shadow.Color = 8618883
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
    end
    object eLinearBorder2: TEdit
      Left = 589
      Top = 10
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2. '#1044#1086#1083#1078#1077#1085' '#1073#1099#1090#1100' '#1084#1077#1085#1100#1096#1077' '#1087#1086#1088#1086#1075#1072' '#1073#1088#1072#1082#1072
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object eLinearBorder1: TEdit
      Left = 357
      Top = 10
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1086#1090#1073#1088#1072#1082#1086#1074#1082#1080' '#1074' '#1087#1088#1086#1094#1077#1085#1090#1072#1093' '#1086#1090' '#1091#1088#1086#1074#1085#1103' '#1089#1080#1075#1085#1072#1083#1072
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object cbLinear: TCheckBox
      Left = 104
      Top = 19
      Width = 97
      Height = 23
      Caption = #1042' '#1088#1072#1073#1086#1090#1077
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 5
      Visible = False
      OnKeyPress = FormKeyPress
    end
    object eLinearDeadZoneFinish: TEdit
      Left = 1202
      Top = 10
      Width = 73
      Height = 27
      Hint = #1053#1077#1082#1086#1085#1090#1088#1086#1083#1080#1088#1091#1077#1084#1072#1103' '#1079#1086#1085#1072' '#1074' '#1082#1086#1085#1094#1077' '#1090#1088#1091#1073#1099', '#1074' '#1084#1084
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object eLinearDeadZoneStart: TEdit
      Left = 941
      Top = 10
      Width = 73
      Height = 27
      Hint = #1053#1077#1082#1086#1085#1090#1088#1086#1083#1080#1088#1091#1077#1084#1072#1103' '#1079#1086#1085#1072' '#1074' '#1085#1072#1095#1072#1083#1077' '#1090#1088#1091#1073#1099', '#1074' '#1084#1084
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
  end
  object gbSummary: TGroupBox
    Left = 8
    Top = 539
    Width = 1353
    Height = 148
    Caption = #1048#1090#1086#1075
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    object lResult: TLabel
      Left = 380
      Top = 17
      Width = 87
      Height = 23
      Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 571
      Top = 16
      Width = 52
      Height = 23
      Caption = #1056#1077#1079' 1:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lCut1: TLabel
      Left = 642
      Top = 16
      Width = 42
      Height = 23
      Caption = 'lCut1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 732
      Top = 16
      Width = 52
      Height = 23
      Caption = #1056#1077#1079' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lCut2: TLabel
      Left = 796
      Top = 16
      Width = 42
      Height = 23
      Caption = 'lCut2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 943
      Top = 16
      Width = 84
      Height = 19
      Caption = #1052#1080#1085' '#1075#1086#1076#1085#1086':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SummaryChart: TChart
      Left = 2
      Top = 57
      Width = 1349
      Height = 89
      BackWall.Transparent = False
      BackWall.Shadow.Visible = True
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 0
      MarginRight = 0
      MarginTop = 0
      Title.Text.Strings = (
        '')
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Maximum = 5.000000000000000000
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 4.249999999999990000
      DepthAxis.Minimum = 3.249999999999989000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 4.249999999999990000
      DepthTopAxis.Minimum = 3.249999999999989000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Maximum = 700.800000000000800000
      LeftAxis.Minimum = 460.800000000000200000
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      View3D = False
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      OnClick = ChartClick
      ColorPaletteIndex = 13
      object Series4: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        BarWidthPercent = 100
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
    end
    object eMin_Good_Length: TEdit
      Left = 1044
      Top = 12
      Width = 73
      Height = 27
      Hint = 
        #1052#1080#1085#1080#1084#1072#1083#1100#1085#1099#1081' '#1075#1086#1076#1085#1099#1081' '#1091#1095#1072#1089#1090#1086#1082' '#1076#1083#1103' '#1086#1090#1088#1077#1079#1082#1080' '#1090#1088#1091#1073#1099' '#1074' '#1079#1086#1085#1072#1093'. '#1055#1086#1089#1090#1072#1074#1100#1090#1077' ' +
        '"0" '#1076#1083#1103' '#1085#1077#1088#1077#1084#1086#1085#1090#1080#1088#1091#1077#1084#1086#1081' '#1090#1088#1091#1073#1099
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = eSettingsChange
      OnKeyPress = FormKeyPress
    end
    object pSolidGroup: TPanel
      Left = 230
      Top = 17
      Width = 89
      Height = 25
      ParentCustomHint = False
      BiDiMode = bdLeftToRight
      Caption = #1043#1055
      Ctl3D = True
      DoubleBuffered = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentDoubleBuffered = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 2
    end
    object bGoodTube: TButton
      Left = 56
      Top = 17
      Width = 75
      Height = 25
      Caption = #1043#1086#1076#1085#1086
      TabOrder = 3
      OnClick = bGoodTubeClick
    end
    object bBadTube: TButton
      Left = 139
      Top = 17
      Width = 75
      Height = 25
      Caption = #1041#1088#1072#1082
      TabOrder = 4
      OnClick = bBadTubeClick
    end
  end
  object gbThickness: TGroupBox
    Left = 8
    Top = 359
    Width = 1353
    Height = 176
    Caption = #1058#1086#1083#1097#1080#1085#1086#1084#1077#1090#1088#1080#1103
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    object Label4: TLabel
      Left = 253
      Top = 15
      Width = 98
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1073#1088#1072#1082#1072':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 466
      Top = 15
      Width = 117
      Height = 19
      Caption = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object ThicknessChart: TChart
      Left = 2
      Top = 64
      Width = 1349
      Height = 110
      BackWall.Transparent = False
      BackWall.Shadow.Visible = True
      Foot.AdjustFrame = False
      Foot.Alignment = taLeftJustify
      Foot.Visible = False
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 0
      MarginRight = 0
      MarginTop = 1
      PrintProportional = False
      SubFoot.Alignment = taLeftJustify
      SubFoot.Visible = False
      SubTitle.Alignment = taLeftJustify
      SubTitle.Visible = False
      Title.Alignment = taLeftJustify
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Axis.Width = 1
      BottomAxis.ExactDateTime = False
      BottomAxis.Increment = 1.000000000000000000
      BottomAxis.LabelsSeparation = 0
      BottomAxis.Maximum = 8.000000000000000000
      BottomAxis.MaximumOffset = 1
      BottomAxis.MinorTickCount = 0
      BottomAxis.EndPosition = 99.000000000000000000
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.ExactDateTime = False
      DepthAxis.Increment = 1.000000000000000000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.ExactDateTime = False
      DepthTopAxis.Increment = 1.000000000000000000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 1.000000000000000000
      LeftAxis.LabelsMultiLine = True
      LeftAxis.Maximum = 903.599999999995800000
      LeftAxis.Minimum = 663.599999999997300000
      Pages.AutoScale = True
      Pages.MaxPointsPerPage = 7
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      RightAxis.Visible = False
      Shadow.Color = 9408399
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Visible = False
      View3D = False
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      PrintMargins = (
        15
        15
        15
        45)
      ColorPaletteIndex = 13
      object Series3: TBarSeries
        Depth = 0
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Margins.Left = 44
        Marks.Visible = False
        ShowInLegend = False
        AutoMarkPosition = False
        BarRound = brNone
        BarWidthPercent = 100
        Dark3D = False
        MarksOnBar = True
        MultiBar = mbStacked
        OffsetPercent = 50
        YOrigin = 3.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          0007000000000000000050894000000000004887400000000000A88640000000
          000008864000000000006885400000000000F084400000000000D08140}
      end
    end
    object eThicknessBorder2: TEdit
      Left = 589
      Top = 10
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1082#1083#1072#1089#1089#1072' 2. '#1044#1086#1083#1078#1077#1085' '#1073#1099#1090#1100' '#1073#1086#1083#1100#1096#1077' '#1087#1086#1088#1086#1075#1072' '#1073#1088#1072#1082#1072
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnChange = eSettingsChange
    end
    object eThicknessBorder1: TEdit
      Left = 357
      Top = 10
      Width = 73
      Height = 27
      Hint = #1055#1086#1088#1086#1075' '#1086#1090#1073#1088#1072#1082#1086#1074#1082#1080' '#1074' '#1084#1084' '
      NumbersOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = eSettingsChange
    end
    object cbThickness: TCheckBox
      Left = 104
      Top = 17
      Width = 97
      Height = 27
      Caption = #1042' '#1088#1072#1073#1086#1090#1077
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 3
      Visible = False
      OnKeyPress = FormKeyPress
    end
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 802
    Width = 1278
    Height = 31
    Panels = <
      item
        Width = 320
      end
      item
        Text = #1043#1086#1090#1086#1074' '#1082' '#1088#1072#1073#1086#1090#1077
        Width = 400
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
  end
  object StatusBarTop: TStatusBar
    Left = 0
    Top = 0
    Width = 1284
    Height = 42
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Text = '           '#1058#1080#1087#1086#1088#1072#1079#1084#1077#1088': '
        Width = 300
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
    UseSystemFont = False
  end
  object cbTypeSize: TComboBox
    Left = 168
    Top = 8
    Width = 113
    Height = 27
    Style = csDropDownList
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnKeyPress = FormKeyPress
    OnSelect = cbTypeSizeSelect
  end
  object bManage: TButton
    Left = 757
    Top = 55
    Width = 119
    Height = 41
    Caption = 'F8 '#1053#1072#1083#1072#1076#1082#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = bManageClick
    OnKeyPress = FormKeyPress
  end
  object Button1: TButton
    Left = 882
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 17
    Visible = False
    OnClick = Button1Click
    OnKeyPress = FormKeyPress
  end
  object Edit1: TEdit
    Left = 942
    Top = 88
    Width = 73
    Height = 21
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    NumbersOnly = True
    ParentFont = False
    TabOrder = 18
    Text = '12'
    Visible = False
  end
  object bCrashForward: TButton
    Left = 451
    Top = 55
    Width = 171
    Height = 41
    Caption = 'F6 '#1042#1099#1075#1086#1085' / '#1058#1088#1072#1085#1079#1080#1090
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clSkyBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = bCrashForwardClick
    OnKeyPress = FormKeyPress
  end
  object bReturnMode: TButton
    Left = 632
    Top = 55
    Width = 119
    Height = 41
    Caption = 'F7 '#1042#1086#1079#1074#1088#1072#1090
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = bReturnModeClick
    OnKeyPress = FormKeyPress
  end
  object Memo1: TMemo
    Left = 1052
    Top = 230
    Width = 216
    Height = 230
    ScrollBars = ssVertical
    TabOrder = 19
    Visible = False
  end
  object bCancelWork: TButton
    Left = 331
    Top = 55
    Width = 114
    Height = 41
    Caption = #1054#1090#1084#1077#1085#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = bCancelWorkClick
    OnKeyPress = FormKeyPress
  end
  object cbInterruptView: TCheckBox
    Left = 180
    Top = 102
    Width = 147
    Height = 17
    Caption = #1055#1088#1077#1088#1099#1074#1072#1085#1080#1077' '#1085#1072' '#1087#1088#1086#1089#1084#1086#1090#1088
    TabOrder = 7
  end
  object cbEtalon: TCheckBox
    Left = 453
    Top = 102
    Width = 72
    Height = 17
    Caption = #1069#1090#1072#1083#1086#1085
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
    OnClick = cbEtalonClick
  end
  object pStatistics: TPanel
    Left = 1013
    Top = 48
    Width = 268
    Height = 82
    TabOrder = 6
    object eValid: TLabeledEdit
      Left = 201
      Top = 4
      Width = 56
      Height = 21
      Color = clLime
      EditLabel.Width = 70
      EditLabel.Height = 13
      EditLabel.Caption = #1043#1086#1076#1085#1099#1093' '#1090#1088#1091#1073':'
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clGreen
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = []
      EditLabel.ParentFont = False
      LabelPosition = lpLeft
      NumbersOnly = True
      ReadOnly = True
      TabOrder = 1
    end
    object eBrack: TLabeledEdit
      Left = 201
      Top = 30
      Width = 56
      Height = 21
      Color = clRed
      EditLabel.Width = 99
      EditLabel.Height = 13
      EditLabel.Caption = #1041#1088#1072#1082#1086#1074#1072#1085#1085#1099#1093' '#1090#1088#1091#1073':'
      EditLabel.Color = clBlack
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clRed
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = []
      EditLabel.ParentColor = False
      EditLabel.ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentFont = False
      ReadOnly = True
      TabOrder = 2
    end
    object bStatsToNull: TButton
      Left = 8
      Top = 8
      Width = 81
      Height = 31
      Caption = #1054#1073#1085#1091#1083#1080#1090#1100
      TabOrder = 0
      OnClick = bStatsToNullClick
    end
    object eBrackInDaStreet: TLabeledEdit
      Left = 201
      Top = 57
      Width = 56
      Height = 21
      ParentCustomHint = False
      BiDiMode = bdLeftToRight
      Color = clFuchsia
      Ctl3D = True
      DoubleBuffered = False
      EditLabel.Width = 70
      EditLabel.Height = 13
      EditLabel.Caption = #1050#1072#1088#1084#1072#1085' '#1073#1088#1072#1082#1072
      EditLabel.Color = clNavy
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clFuchsia
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = []
      EditLabel.ParentColor = False
      EditLabel.ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentDoubleBuffered = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 3
      OnChange = eBrackInDaStreetChange
    end
  end
  object Button2: TButton
    Left = 942
    Top = 64
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 20
    Visible = False
    OnClick = Button2Click
  end
  object cbNoMarking: TCheckBox
    Left = 340
    Top = 102
    Width = 108
    Height = 17
    Cursor = crHandPoint
    Hint = #1056#1072#1073#1086#1090#1072' '#1073#1077#1079' '#1084#1072#1088#1082#1080#1088#1086#1074#1082#1080' '#1076#1077#1092#1077#1082#1090#1086#1074
    Caption = #1041#1077#1079' '#1084#1072#1088#1082#1080#1088#1086#1074#1082#1080
    Color = clRed
    Ctl3D = True
    DoubleBuffered = True
    Font.Charset = BALTIC_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentCtl3D = False
    ParentDoubleBuffered = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
    OnClick = cbNoMarkingClick
  end
  object bManualSG: TButton
    Left = 892
    Top = 87
    Width = 75
    Height = 25
    Caption = #1043#1088#1091#1087#1087#1072
    TabOrder = 22
    Visible = False
    OnClick = bManualSGClick
  end
  object MainMenu1: TMainMenu
    Left = 1088
    object menuJob: TMenuItem
      Caption = #1056#1072#1073#1086#1090#1072
      object menuWork: TMenuItem
        Caption = #1056#1072#1073#1086#1090#1072
        ShortCut = 116
        OnClick = bWorkClick
      end
      object menuCrashForward: TMenuItem
        Caption = #1042#1099#1075#1086#1085
        ShortCut = 117
        OnClick = bCrashForwardClick
      end
      object menuReturnMode: TMenuItem
        Caption = #1042#1086#1079#1074#1088#1072#1090
        ShortCut = 118
        OnClick = bReturnModeClick
      end
      object menuManagement: TMenuItem
        Caption = #1053#1072#1083#1072#1076#1082#1072
        ShortCut = 119
        OnClick = bManageClick
      end
      object menuQuit: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = menuQuitClick
      end
    end
    object menuSettings: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
      object menuTypeSize: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1090#1080#1087#1086#1088#1072#1079#1084#1077#1088#1072
        ShortCut = 115
        OnClick = menuTypeSizeClick
      end
      object menuMeasures: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
        ShortCut = 8307
        OnClick = menuMeasuresClick
      end
      object menuSolidGroup: TMenuItem
        Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
        ShortCut = 16499
        OnClick = menuSolidGroupClick
      end
      object menuColors: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1094#1074#1077#1090#1072
        OnClick = menuColorsClick
      end
      object menuViewEtalons: TMenuItem
        Caption = #1055#1088#1086#1089#1084#1086#1090#1088' '#1101#1090#1072#1083#1086#1085#1086#1074' '#1043#1055
        ShortCut = 16498
        OnClick = menuViewEtalonsClick
      end
    end
    object menuTube: TMenuItem
      Caption = #1058#1088#1091#1073#1072
      object menuView: TMenuItem
        Caption = #1055#1088#1086#1089#1084#1086#1090#1088
        ShortCut = 114
        OnClick = bViewClick
      end
      object menuClearCharts: TMenuItem
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100
        ShortCut = 121
        OnClick = ClearCharts
      end
      object menuSaveTube: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        ShortCut = 16467
        OnClick = menuSaveTubeClick
      end
      object menuLoadTube: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        ShortCut = 16463
        OnClick = menuLoadTubeClick
      end
    end
    object menuDiagnostics: TMenuItem
      Caption = #1044#1080#1072#1075#1085#1086#1089#1090#1080#1082#1072
      object menuTest: TMenuItem
        Caption = #1058#1077#1089#1090
        ShortCut = 113
        OnClick = bTestClick
      end
      object menuSGTest: TMenuItem
        Caption = #1058#1077#1089#1090' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
        ShortCut = 16497
        OnClick = menuSGTestClick
      end
      object menuTestAdvantech: TMenuItem
        Caption = #1058#1077#1089#1090' '#1091#1089#1090#1088#1086#1081#1089#1090#1074
        ShortCut = 8310
        Visible = False
        OnClick = menuTestAdvantechClick
      end
      object menuTestASUConnection: TMenuItem
        Caption = #1058#1077#1089#1090' '#1089#1074#1103#1079#1080' '#1089' '#1040#1057#1059
        OnClick = menuTestASUConnectionClick
      end
      object menuSignalsState: TMenuItem
        Caption = #1057#1080#1075#1085#1072#1083#1099
        ShortCut = 120
        OnClick = menuSignalsStateClick
      end
      object menuProtocol: TMenuItem
        Caption = #1055#1088#1086#1090#1086#1082#1086#1083
        ShortCut = 122
        OnClick = menuProtocolClick
      end
    end
    object menuHelp: TMenuItem
      Caption = #1055#1086#1084#1086#1097#1100
      ShortCut = 112
      object menuF1: TMenuItem
        Caption = #1057#1087#1088#1072#1074#1082#1072
        ShortCut = 112
        OnClick = menuF1Click
      end
      object N1: TMenuItem
        Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
        OnClick = N1Click
      end
    end
  end
  object SaveToFileDialog: TSaveDialog
    DefaultExt = '.dkb'
    Filter = #1044#1072#1085#1085#1099#1077' '#1087#1086' '#1090#1088#1091#1073#1077'(.dkb)|*.dkb|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofHideReadOnly, ofNoChangeDir, ofShowHelp, ofEnableSizing]
    Left = 912
  end
  object OpenDialogFromFile: TOpenDialog
    DefaultExt = '.dkb'
    Filter = #1044#1072#1085#1085#1099#1077' '#1087#1086' '#1090#1088#1091#1073#1077'(.dkb)|*.dkb'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 1008
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 816
  end
  object MixingTimer: TTimer
    Enabled = False
    OnTimer = MixingTimerTimer
    Left = 736
  end
end
