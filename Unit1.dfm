object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'RLE Archivator'
  ClientHeight = 437
  ClientWidth = 1020
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
  object Image1: TImage
    Left = 670
    Top = 79
    Width = 331
    Height = 282
    Stretch = True
  end
  object Label1: TLabel
    Left = 816
    Top = 392
    Width = 5
    Height = 22
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Button1: TButton
    Left = 256
    Top = 372
    Width = 129
    Height = 57
    Caption = 'Archive file'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 544
    Top = 372
    Width = 129
    Height = 57
    Caption = 'Dearchive file'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 87
    Width = 289
    Height = 274
    ScrollBars = ssBoth
    TabOrder = 2
  end
  object Memo2: TMemo
    Left = 360
    Top = 87
    Width = 289
    Height = 274
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object Button4: TButton
    Left = 303
    Top = 128
    Width = 50
    Height = 25
    Caption = '-->'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 304
    Top = 296
    Width = 50
    Height = 25
    Caption = '<--'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = Button5Click
  end
  object OpenDialog1: TOpenDialog
    Left = 544
  end
end
