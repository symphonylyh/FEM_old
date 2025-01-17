unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, dxNavBarCollns, dxNavBarBase, ExtCtrls, dxNavBar, cxControls,
  cxPC, cxGraphics, dxStatusBar, dxExEdtr, dxCntner, dxInspct, StdCtrls,
  cxSplitter, cxGroupBox, dxInspRw, dxBar, dxDockControl, dxDockPanel,
  cxContainer, cxEdit, cxTextEdit, cxMaskEdit, cxSpinEdit,
  cxLookAndFeelPainters, cxButtons,Math, cxMemo,dxOffice11, ImgList,
  ComCtrls, cxProgressBar, IniFiles, dxBarExtItems;

const
  ProjectPanelID=0;
  ActionPanelID=1;
  DateTimePanelID=2;

  WM_DISPLAYLAYER=WM_USER + 1;
  WM_REFRESHZOOM=WM_USER + 2;
const
  ProjectSign='GTPV';
  LayerSign=' LYR';
  ProjectFileVersion=1.0;
  LayerFileVersion=1.0;

type
  TDisplayInfo=record
  ShowAC:boolean;
  ShowBase:boolean;
  ShowSubbase:boolean;
  ShowSubgrade:boolean;
  ACSize:integer;
  BaseSize:integer;
  SubbaseSize:integer;
  SubgradeSize:integer;
  MeshWidth:integer;
  PixelPerInch:Integer; //Ger.ek boyu i�in 300 yaz�lmal�, ama biz 30 yapsak san�r�m yeter
end;

type
  TDoublePoint=record
  Row:double;
  Col:double;
end;
type PDoublePoint=^TDoublePoint;

type
  TProjectInfo=record
  Title:array [0..50]of char;
  Creator:array [0..50]of char;
  Name:array[0..50]of char;
  Description:array[0..250]of char;
end;
type PProjectInfo=^TProjectInfo;

type
  TMeshInfo=record
  RowCount:integer;
  ColCount:integer;
  Width:double;
  Depth:double;
  Mesh:array of array of TDoublePoint;
  IsScaled:boolean;
end;
type PMeshInfo=^TMeshInfo;

type
  TWheelData=record
  Radius:double;
  ContactPressure:double;
  WheelLoad:double;
end;
type PWheelData=^TWheelData;

type
  TIsotropicLayer=record
  Modulus:double;
  Poissons_Ratio:double;
  BodyForce:double;
  InternalFriction:double;
  Cohesion:double;
  RestStress:double;
end;
type PIsotropicLayer=^TIsotropicLayer;

type
  TAnisotropicLayer=record
  ModulusVertical:double;
  PoissonHorizontal:double;
  PoissonVertical:double;
  HorzToVertModular:double;
  ShearToVertModular:double;
  BodyForce:double;
  InternalFriction:double;
  Cohesion:double;
  RestStress:double;
end;
type PAnisotropicLayer=^TAnisotropicLayer;

type
  TKThetaModel=record
  K1V:double;
  K2V:double;
  K1H:double;
  K2H:double;
  K1S:double;
  K2S:double;
end;
type PKThetaModel=^TKThetaModel;
type
  TUzanModel=record
  K1V:double;
  K2V:double;
  K3V:double;
  K1H:double;
  K2H:double;
  K3H:double;
  K1S:double;
  K2S:double;
  K3S:double;
end;
type PUzanModel=^TUzanModel;

type
  TBilinearModel=record
  K1:double;
  K2:double;
  K3:double;
  K4:double;
  Devlow:double;
  Devhigh:double;
end;
type PBilinearMode=^TBilinearModel;

type
  TBrownModel=record
  C:double;
  D:double;
end;
type PBrownModel=^TBrownModel;

type TLayerType=(ltAC,ltBase,ltSubbase,ltSubgrade);
type
  TLayer=record
  LayerType:TLayerType;
  Thickness:double;
  IsIsotropic:boolean;
  Isotropic:TIsotropicLayer;
  Anisotropic:TAnisotropicLayer;
  IsKThetaModel:boolean;
  KThetaModel:TKThetaModel;
  UzanModel:TUzanModel;
  BilinearModel:TBilinearModel;
  BrownModel:TBrownModel;
end;
type PLayer=^TLayer;

type PDouble=^double;
type
  TLayerWithPreview=record
  Layer:TLayer;
  Preview:TPaintBox;
  Caption:string;
  FontColor:TColor;
  IsScaled:boolean;
end;
type PLayerWithPreview=^TLayerWithPreview;

type
  TfrmMain = class(TForm)
    nbNavigator: TdxNavBar;
    nbgProject: TdxNavBarGroup;
    nbgAnalysis: TdxNavBarGroup;
    nbiWheelLoadData: TdxNavBarItem;
    nbiLayersMaterials: TdxNavBarItem;
    nbiExecutionDetails: TdxNavBarItem;
    nbiGeoGrid: TdxNavBarItem;
    nbiPerformAnalysis: TdxNavBarItem;
    PageControl: TcxPageControl;
    sbStatus: TdxStatusBar;
    tmrDateTime: TTimer;
    tbLayers: TcxTabSheet;
    nbgCommon: TdxNavBarGroup;
    tbNewProject: TcxTabSheet;
    dxLayerInspector: TdxInspector;
    cxSplitter1: TcxSplitter;
    MainSplitter: TcxSplitter;
    pnlCaption1: TPanel;
    ImgGradient1: TImage;
    lblLayerAndMaterials: TLabel;
    Label4: TLabel;
    lblAddLayer: TLabel;
    lblDeleteLayer: TLabel;
    pbLayerPreview: TPaintBox;
    dxLayerInspectorRow1: TdxInspectorTextRow;
    dxLayerType: TdxInspectorTextPickRow;
    dxLayerThickness: TdxInspectorTextSpinRow;
    dxLayerStyle: TdxInspectorTextPickRow;
    dxLayerModel: TdxInspectorTextPickRow;
    dxIsotropicProperties: TdxInspectorTextRow;
    dxIsoModulus: TdxInspectorTextSpinRow;
    dxIsoPoisson: TdxInspectorTextSpinRow;
    dxIsoBodyForce: TdxInspectorTextSpinRow;
    dxIsoCohesion: TdxInspectorTextSpinRow;
    dxIsoRest: TdxInspectorTextSpinRow;
    dxAnisotropicProperties: TdxInspectorTextRow;
    dxAnisoVertModulus: TdxInspectorTextSpinRow;
    dxAnisoHorzPoisson: TdxInspectorTextSpinRow;
    dxAnisoVertPoisson: TdxInspectorTextSpinRow;
    dxAnisoHorzToVert: TdxInspectorTextSpinRow;
    dxAnisoBodyforce: TdxInspectorTextSpinRow;
    dxAnisoFriction: TdxInspectorTextSpinRow;
    dxAnisoCohesion: TdxInspectorTextSpinRow;
    dxAnisoRest: TdxInspectorTextSpinRow;
    dxKTheteProperties: TdxInspectorTextRow;
    dxLayerInspectorRow24: TdxInspectorTextSpinRow;
    dxKThetaKV1: TdxInspectorTextSpinRow;
    dxKThetaKV2: TdxInspectorTextSpinRow;
    dxLayerInspectorRow27: TdxInspectorTextRow;
    dxKThetaKH1: TdxInspectorTextSpinRow;
    dxKThetaKH2: TdxInspectorTextSpinRow;
    dxLayerInspectorRow30: TdxInspectorTextRow;
    dxKThetaKS1: TdxInspectorTextSpinRow;
    dxKThetaKS2: TdxInspectorTextSpinRow;
    dxUzanProperties: TdxInspectorTextRow;
    dxLayerInspectorRow34: TdxInspectorTextRow;
    dxLayerInspectorRow35: TdxInspectorTextRow;
    dxLayerInspectorRow36: TdxInspectorTextRow;
    dxUzanV1: TdxInspectorTextSpinRow;
    dxUzanH1: TdxInspectorTextSpinRow;
    dxUzanH2: TdxInspectorTextSpinRow;
    dxUzanH3: TdxInspectorTextSpinRow;
    dxUzanS1: TdxInspectorTextSpinRow;
    dxUzanS2: TdxInspectorTextSpinRow;
    dxUzanS3: TdxInspectorTextSpinRow;
    dxIsoFriction: TdxInspectorTextSpinRow;
    dxAnisoShearToVert: TdxInspectorTextSpinRow;
    dxUzanV2: TdxInspectorTextSpinRow;
    dxUzanV3: TdxInspectorTextSpinRow;
    dxBarManager: TdxBarManager;
    mnuFileNewProject: TdxBarButton;
    mnuFile: TdxBarSubItem;
    mnuFileOpen: TdxBarButton;
    mnuFileSave: TdxBarButton;
    mnuFileSaveAs: TdxBarButton;
    mnuFileClose: TdxBarButton;
    mnuFileExit: TdxBarButton;
    mnuInput: TdxBarSubItem;
    mnuViewNavBar: TdxBarButton;
    mnuViewStatusBar: TdxBarButton;
    dxBarSubItem1: TdxBarSubItem;
    mnuViewMainToolbar: TdxBarButton;
    mnuInputWheel: TdxBarButton;
    mnuProjectLayers: TdxBarButton;
    mnuViewGeogrid: TdxBarButton;
    mnuProjectExecutionDetails: TdxBarButton;
    mnuAnalysis: TdxBarSubItem;
    mnuAnalysisPerform: TdxBarButton;
    dxBarSubItem2: TdxBarSubItem;
    mnuHelpHelp: TdxBarButton;
    mnuHelpAbout: TdxBarButton;
    mnuViewPreferences: TdxBarButton;
    nbiOpenProject: TdxNavBarItem;
    nbiNewProject: TdxNavBarItem;
    nbiSaveProject: TdxNavBarItem;
    mnuAnalysisPrint: TdxBarButton;
    nbiPrintReport: TdxNavBarItem;
    dxDockSite1: TdxDockSite;
    dxDockingManager1: TdxDockingManager;
    pnlLayerPreviewBG: TdxDockPanel;
    dxLayoutDockSite1: TdxLayoutDockSite;
    tbExecutionDetails: TcxTabSheet;
    pnlCaption3: TPanel;
    ImgGradient3: TImage;
    lblExecutionDetails: TLabel;
    Label5: TLabel;
    pnlCaption2: TPanel;
    ImgGradient2: TImage;
    lblWelcome: TLabel;
    Label7: TLabel;
    ImgLogo1: TImage;
    mnuAnalysisSystemPreview: TdxBarButton;
    nbiSystemPreview: TdxNavBarItem;
    Label1: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    lblStart: TLabel;
    Label12: TLabel;
    Label14: TLabel;
    tsWheelLoad: TcxTabSheet;
    txtRadius: TcxSpinEdit;
    txtLoad: TcxSpinEdit;
    txtContact: TcxSpinEdit;
    Label13: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    pnlCaption4: TPanel;
    Label20: TLabel;
    cmdApplyWheel: TcxButton;
    cmdRestoreWheel: TcxButton;
    lblMoveLayerUp: TLabel;
    lblMoveLayerDown: TLabel;
    Label21: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    txtAnalyseTitle: TcxTextEdit;
    txtCreatorName: TcxTextEdit;
    txtProjectName: TcxTextEdit;
    txtProjectDescription: TcxMemo;
    cmdApplyExecution: TcxButton;
    cmdRestoreExecution: TcxButton;
    dlgSave: TSaveDialog;
    ImgGradient4: TImage;
    lblWheelLoad: TLabel;
    ImgLogo2: TImage;
    ImgLogo3: TImage;
    ImgLogo4: TImage;
    dxBarSubItem3: TdxBarSubItem;
    mnuProjectLayerWizard: TdxBarButton;
    mnuProjectAC: TdxBarButton;
    mnuProjectBase: TdxBarButton;
    mnuProjectSubbase: TdxBarButton;
    mnuProjectSubgrade: TdxBarButton;
    mnuProjectDeleteLayer: TdxBarButton;
    mnuProjectMoveLayerUp: TdxBarButton;
    mnuProjectMoveLayerDown: TdxBarButton;
    mnuProjectExportLayer: TdxBarButton;
    mnuProjectImportLayer: TdxBarButton;
    dlgImportLayer: TOpenDialog;
    dlgExportLayer: TSaveDialog;
    ilMenu: TImageList;
    dlgOpen: TOpenDialog;
    tbOpenProject: TcxTabSheet;
    pnlCaption5: TPanel;
    ImgGradient5: TImage;
    lblOpen: TLabel;
    lblOpenCaption: TLabel;
    ImgLogo5: TImage;
    Label6: TLabel;
    Label19: TLabel;
    Label26: TLabel;
    Label27: TLabel;
    Label28: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Label25: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    dxLayerInspectorRow46: TdxInspectorTextRow;
    dxBilinearK1: TdxInspectorTextSpinRow;
    dxBilinearK2: TdxInspectorTextSpinRow;
    dxBilinearK3: TdxInspectorTextSpinRow;
    dxBilinearK4: TdxInspectorTextSpinRow;
    dxBilinearDevlow: TdxInspectorTextSpinRow;
    dxBilinearDevHigh: TdxInspectorTextSpinRow;
    dxLayerInspectorRow53: TdxInspectorTextRow;
    dxBrownC: TdxInspectorTextSpinRow;
    dxBrownD: TdxInspectorTextSpinRow;
    Button1: TButton;
    Button2: TButton;
    pnlLayerPreview: TPanel;
    sbPreviewScroll: TScrollBar;
    mnuZoomIn: TdxBarButton;
    mnuZoomOut: TdxBarButton;
    mnuProjectUndo: TdxBarButton;
    mnuViewZoom: TdxBarStatic;
    pbHorizontalRuler: TPaintBox;
    pbVerticalRuler: TPaintBox;
    Panel1: TPanel;
    pbWheel: TPaintBox;
    procedure FormCreate(Sender: TObject);
    procedure tmrDateTimeTimer(Sender: TObject);
    procedure lblAddLayerMouseEnter(Sender: TObject);
    procedure lblAddLayerMouseLeave(Sender: TObject);
    procedure lblDeleteLayerMouseDown(Sender: TObject;
      Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure lblAddLayerMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pbLayerPreviewPaint(Sender: TObject);
    procedure pbLayerPreviewClick(Sender: TObject);
    procedure gbLayerPreviewClick(Sender: TObject);
    procedure mnuInputWheelClick(Sender: TObject);
    procedure mnuProjectLayersClick(Sender: TObject);
    procedure pnlCaption1Resize(Sender: TObject);
    procedure mnuFileExitClick(Sender: TObject);
    procedure mnuFileNewProjectClick(Sender: TObject);
    procedure mnuProjectExecutionDetailsClick(Sender: TObject);
    procedure PageControlPageChanging(Sender: TObject;
      NewPage: TcxTabSheet; var AllowChange: Boolean);
    procedure mnuFileOpenClick(Sender: TObject);
    procedure mnuHelpHelpClick(Sender: TObject);
    procedure pnlLayerPreviewBGMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure txtRadiusPropertiesChange(Sender: TObject);
    procedure txtContactPropertiesChange(Sender: TObject);
    procedure txtLoadPropertiesChange(Sender: TObject);
    procedure cmdRestoreWheelClick(Sender: TObject);
    procedure cmdApplyWheelClick(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure cmdApplyExecutionClick(Sender: TObject);
    procedure cmdRestoreExecutionClick(Sender: TObject);
    procedure txtAnalyseTitlePropertiesChange(Sender: TObject);
    procedure mnuViewMainToolbarClick(Sender: TObject);
    procedure dxBarManagerBarVisibleChange(Sender: TdxBarManager;
      ABar: TdxBar);
    procedure mnuViewStatusBarClick(Sender: TObject);
    procedure mnuViewNavBarClick(Sender: TObject);
    procedure mnuFileCloseClick(Sender: TObject);
    procedure mnuFileSaveClick(Sender: TObject);
    procedure mnuFileSaveAsClick(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure mnuHelpAboutClick(Sender: TObject);
    procedure dxLayerInspectorEdited(Sender: TObject;
      Node: TdxInspectorNode; Row: TdxInspectorRow);
    procedure dxLayerStyleChange(Sender: TObject);
    procedure dxLayerModelChange(Sender: TObject);
    procedure mnuProjectLayerWizardClick(Sender: TObject);
    procedure mnuProjectMoveLayerUpClick(Sender: TObject);
    procedure mnuProjectMoveLayerDownClick(Sender: TObject);
    procedure mnuProjectDeleteLayerClick(Sender: TObject);
    procedure dxLayerInspectorDrawCaption(Sender: TdxInspectorRow;
      ACanvas: TCanvas; ARect: TRect; var AText: String; AFont: TFont;
      var AColor: TColor; var ADone: Boolean);
    procedure mnuProjectExportLayerClick(Sender: TObject);
    procedure mnuProjectImportLayerClick(Sender: TObject);
    procedure mnuProjectACClick(Sender: TObject);
    procedure Label31Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure pnlLayerPreviewResize(Sender: TObject);
    procedure sbPreviewScrollScroll(Sender: TObject;
      ScrollCode: TScrollCode; var ScrollPos: Integer);
    procedure mnuZoomInClick(Sender: TObject);
    procedure mnuZoomOutClick(Sender: TObject);
    procedure mnuProjectUndoClick(Sender: TObject);
    procedure pbHorizontalRulerPaint(Sender: TObject);
    procedure sbPreviewScrollChange(Sender: TObject);
    procedure pbVerticalRulerPaint(Sender: TObject);
    procedure pbWheelPaint(Sender: TObject);
  private
    FModified: boolean;
    FActiveLayer: PLayerWithPreview;
    FLayerPreviewZoom: double;
    FResizing:boolean;

    RestoreOnProgress:boolean;
    procedure AddToHistory;
    procedure StepBackFromHistory;
    procedure ClearHistory;

    procedure DrawArrow(ACanvas:TCanvas;X:integer;Y:integer;Scale:integer);
    procedure DrawWheel(ACanvas:TCanvas;X:integer;Y:integer);
    procedure SetModified(const Value: boolean);
    function WheelData_From_Contact:double;
    function Contact_From_WheelData:double;
    property Modified:boolean read FModified write SetModified;
    procedure SetRowTags(FLayer:PLayerWithPreview);
    procedure SetValueFromTag(Row:integer);
    procedure SetActiveLayer(const Value: PLayerWithPreview);
    procedure WmDisplayLayer(var Msg:TMessage);message WM_DISPLAYLAYER;
    procedure WmRefreshZoom(var Msg:TMessage);message WM_REFRESHZOOM;
    function GetFileName(SaveDialog:TSaveDialog;Ext:string):string;
    procedure CreateLayerPreviewComponent(FLayer:PLayerWithPreview);
    procedure AddLayerFromStream(AStream:TMemoryStream);
    procedure AddLayerFromFile(szFileName: string);
    procedure CheckLayerHeights;
    procedure SetProjectFileName(const Value: string);
    procedure SetLayerPreviewZoom(const Value: double);
  private
    BrushBitmaps:array [0..4]of TBitmap;
    WheelBitmap:TBitmap;
    LayerFontColors:array [0..4]of TColor;
    ProjectInfo:TProjectInfo;
    FProjectFileName:string;
    WheelInfo:TWheelData;
    AllowPageChange:boolean;
    UndoList:TList;
    function GetLayerDisplaySize(FLayer:PLayerWithPreview;DPI:integer):integer;
    function CloseProject(DoSave:boolean):boolean;
    procedure StartNewProject;
    procedure SetInspectorRows(FLayer:PLayerWithPreview);
    function SaveProjectToFile(szFileName: string):boolean;
    function SaveProjectToStream(AStream:TMemoryStream):boolean;
    function OpenProjectFromFile(szFileName:string):boolean;
    function OpenProjectFromStream(AStream:TMemoryStream):boolean;
    procedure ScaleLayerPreviews;
    property ActiveLayer:PLayerWithPreview read FActiveLayer write SetActiveLayer;
    property ProjectFileName:string read FProjectFileName write SetProjectFileName;
    property LayerPreviewZoom:double read FLayerPreviewZoom write SetLayerPreviewZoom;
    { Private declarations }
  public
    LayerList:TList;
    Mesh:TMeshInfo;
    DisplayInfo:TDisplayInfo;
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation
uses NewLayer;

{$R *.dfm}

procedure TfrmMain.FormCreate(Sender: TObject);
var
  tmp:TBitmap;
begin
  //Arabirim d�zenlemeleri
  PageControl.Align:=alClient;
  PageControl.HideTabs:=true;

  AllowPageChange:=true;
  PageControl.ActivePage:=tbNewProject;

  pnlLayerPreview.DoubleBuffered:=true;
  pnlLayerPreviewBG.DoubleBuffered:=true;
  pnlCaption1.DoubleBuffered:=true;
  pnlCaption2.DoubleBuffered:=true;
  pnlCaption3.DoubleBuffered:=true;
  pnlCaption4.DoubleBuffered:=true;
  pnlCaption5.DoubleBuffered:=true;

  dxOffice11.CreateOffice11Colors;

  (MainSplitter.HotZone as TcxMediaPlayer9Style).ShadowStartColor:=dxOffice11.dxOffice11DockColor2;
  (MainSplitter.HotZone as TcxMediaPlayer9Style).ShadowHighlightStartColor:=dxOffice11.dxOffice11DockColor1;

  dxLayerInspector.Color:=dxOffice11.dxOffice11DockColor2;
  dxLayerInspector.GridColor:=dxOffice11.dxOffice11DockColor1;

  tmp:=TBitmap.Create;
  tmp.Width:=235;
  tmp.Height:=80;
  tmp.PixelFormat:=pf24bit;
  dxOffice11.FillGradientRect(tmp.Canvas.Handle,Rect(0,0,tmp.Width,tmp.Height),dxOffice11.dxOffice11DockColor2,dxOffice11.dxOffice11DockColor1,true);
  dxOffice11.ReleaseOffice11Colors;

  ImgGradient1.Picture.Bitmap.Assign(tmp);
  ImgGradient2.Picture.Bitmap.Assign(tmp);
  ImgGradient3.Picture.Bitmap.Assign(tmp);
  ImgGradient4.Picture.Bitmap.Assign(tmp);
  ImgGradient5.Picture.Bitmap.Assign(tmp);
  tmp.Free;

  //Brush'lar� olu�tur
  BrushBitmaps[0]:=TBitmap.Create;
  BrushBitmaps[0].LoadFromFile(ExtractFilePath(ParamStr(0)) + 'AC.pat');
  BrushBitmaps[1]:=TBitmap.Create;
  BrushBitmaps[1].LoadFromFile(ExtractFilePath(ParamStr(0)) + 'Base.pat');
  BrushBitmaps[2]:=TBitmap.Create;
  BrushBitmaps[2].LoadFromFile(ExtractFilePath(ParamStr(0)) + 'Subbase.pat');
  BrushBitmaps[3]:=TBitmap.Create;
  BrushBitmaps[3].LoadFromFile(ExtractFilePath(ParamStr(0)) + 'Subgrade.pat');
  LayerFontColors[0]:=clWhite;
  LayerFontColors[1]:=clBlack;
  LayerFontColors[2]:=clBlack;
  LayerFontColors[3]:=clWhite;

  WheelBitmap:=TBitmap.Create;
  WheelBitmap.LoadFromFile(ExtractFilePath(ParamStr(0)) + 'Wheel.pat');

  //Listeleri haz�rla
  AllowPageChange:=false;
  LayerList:=TList.Create;
  UndoList:=TList.Create;
  ActiveLayer:=nil;
  RestoreOnProgress:=false;

  //Yeni proje olu�tur
  StartNewProject;
end;

procedure TfrmMain.tmrDateTimeTimer(Sender: TObject);
begin
  sbStatus.Panels[DateTimePanelID].Text:= DateTimeToStr(Now);
end;

procedure TfrmMain.StartNewProject;
var
  MeshFile:TIniFile;
  I,J:integer;
  DefRow,DefCol:double;
  Total:TDoublePoint;
begin
  //Proje bilgilerini resetle
  StrPCopy(ProjectInfo.Title,'Untitled Project');
  StrPCopy(ProjectInfo.Creator,'Unknown Creator');
  StrPCopy(ProjectInfo.Name,'Unnamed Project');
  StrPCopy(ProjectInfo.Description,'');

  //Mesh Dosyas�n� a�
  MeshFile:=TIniFile.Create(ExtractFilePath(ParamStr(0)) + 'Mesh.ini');

  Mesh.RowCount:=MeshFile.ReadInteger('MeshInfo','RowCount',50);
  Mesh.ColCount:=MeshFile.ReadInteger('MeshInfo','ColCount',16);
  DefRow:=MeshFile.ReadFloat('MeshInfo',' DefaultRowSpace',1);
  DefCol:=MeshFile.ReadFloat('MeshInfo',' DefaultColSpace',1);

  Total.Col:=0;
  SetLength(Mesh.Mesh,Mesh.ColCount);
  for I:=0 to Mesh.ColCount-1 do
  begin
    SetLength(Mesh.Mesh[I],Mesh.RowCount);
    Total.Row:=0;
    Total.Col :=Total.Col + MeshFile.ReadFloat('MeshInfo',' Col' + IntToStr(I+1),DefCol);
    for J:=0 to Mesh.RowCount-1 do
    begin
      Total.Row :=Total.Row + MeshFile.ReadFloat('MeshInfo',' Row' + IntToStr(J+1),DefRow);
      Mesh.Mesh[I,J]:=Total;
    end;
  end;
  Mesh.Depth:=Total.Row;
  Mesh.Width:=Total.Col;

  DisplayInfo.ShowAC:=MeshFile.ReadBool('DISPLAYINFO','ShowAC',true);
  DisplayInfo.ShowBase:=MeshFile.ReadBool('DISPLAYINFO','ShowBase',true);
  DisplayInfo.ShowSubbase:=MeshFile.ReadBool('DISPLAYINFO','ShowSubbase',true);
  DisplayInfo.ShowSubgrade:=MeshFile.ReadBool('DISPLAYINFO','ShowSubgrade',true);
  DisplayInfo.ACSize:=MeshFile.ReadInteger('DISPLAYINFO','ACSize',-1);
  DisplayInfo.BaseSize:=MeshFile.ReadInteger('DISPLAYINFO','BaseSize',-1);
  DisplayInfo.SubbaseSize:=MeshFile.ReadInteger('DISPLAYINFO','SubbaseSize',-1);
  DisplayInfo.SubgradeSize:=MeshFile.ReadInteger('DISPLAYINFO','SubgradeSize',-1);
  DisplayInfo.PixelPerInch:=MeshFile.ReadInteger('DISPLAYINFO','PixelPerInch',72);

  DisplayInfo.MeshWidth:=MeshFile.ReadInteger('DISPLAYINFO','MeshWidth',-1);


  //Wheel Load Default de�erleri
  WheelInfo.Radius:=6;
  WheelInfo.ContactPressure:=80;
  WheelInfo.WheelLoad:=9047.8;


  //Arabirimi ayarla
  LayerPreviewZoom:=1;

  cmdApplyWheel.Enabled:=false;
  cmdRestoreWheel.Enabled:=false;
  cmdApplyExecution.Enabled:=false;
  cmdRestoreExecution.Enabled:=false;


  nbNavigator.Visible:=mnuViewNavBar.Down;
  MainSplitter.Visible:=mnuViewNavBar.Down;
  MainSplitter.Left:=nbNavigator.Width;

  AllowPageChange:=true;
  PageControl.ActivePage:=tbNewProject;
  AllowPageChange:=false;
  PageControl.Visible:=true;

  ProjectFileName:='';
end;

procedure TfrmMain.lblAddLayerMouseEnter(Sender: TObject);
begin
  (Sender as TLabel).Font.Color:=clBlue;
end;

procedure TfrmMain.lblAddLayerMouseLeave(Sender: TObject);
begin
  (Sender as TLabel).Font.Color:=clNavy;
end;

procedure TfrmMain.lblDeleteLayerMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  (Sender as TLabel).Font.Color:=clRed;
end;

procedure TfrmMain.lblAddLayerMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
//  (Sender as TLabel).Font.Color:=clBlue;
end;

procedure TfrmMain.pbLayerPreviewPaint(Sender: TObject);
var
  FLayer:PLayerWithPreview;
  TextHeight:integer;
  TextWidth:integer;
  ARect:TRect;
  AColor:TColor;
  ArrowPos:integer;
begin
  FLayer:=Pointer((Sender as TPaintBox).Tag);

  FLayer.Preview.Canvas.Brush.Style:=bsSolid;
  FLayer.Preview.Canvas.Brush.Bitmap:=BrushBitmaps[integer(FLayer.Layer.LayerType)];
  FLayer.Preview.Canvas.FillRect(FLayer.Preview.ClientRect);
  FLayer.Preview.Canvas.Brush.Bitmap:=nil;

  TextHeight:=FLayer.Preview.Canvas.TextHeight(FLayer.Caption);
  TextWidth:=FLayer.Preview.Canvas.TextWidth(FLayer.Caption);

  //Yaz� Drop Shadow
  FLayer.Preview.Canvas.Font.Name:='Tahoma';
  FLayer.Preview.Canvas.Font.Size:=10;
  FLayer.Preview.Canvas.Font.Style:=[fsBold];
  FLayer.Preview.Canvas.Brush.Style:=bsClear;

  AColor:=LayerFontColors[integer(FLayer.Layer.LayerType)];
  AColor:=RGB(255 - GetRValue(AColor),255 - GetGValue(AColor),255 - GetBValue(AColor));
  FLayer.Preview.Canvas.Font.Color:=AColor ;

  if not FLayer.IsScaled then
    FLayer.Preview.Canvas.TextOut(FLayer.Preview.Width div 2 - TextWidth div 2 + 1,
                                  FLayer.Preview.Height div 2 - TextHeight div 2 + 1,
                                  FLayer.Caption )
  else
    FLayer.Preview.Canvas.TextOut(FLayer.Preview.Width div 2 - TextWidth div 2 + 1,
                                  TextHeight  + 6,
                                  FLayer.Caption );

  FLayer.Preview.Canvas.Font.Color:=LayerFontColors[integer(FLayer.Layer.LayerType)];

  if not FLayer.IsScaled then
    FLayer.Preview.Canvas.TextOut(FLayer.Preview.Width div 2 - TextWidth div 2,
                                  FLayer.Preview.Height div 2 - TextHeight div 2,
                                  FLayer.Caption )
  else
    FLayer.Preview.Canvas.TextOut(FLayer.Preview.Width div 2 - TextWidth div 2,
                                  TextHeight + 5,
                                  FLayer.Caption ); //TextHeight + 5 de�erindeki 5 de�eri Soner'den


  if ActiveLayer=FLayer then
  begin
    ARect:=Rect(1,1,FLayer.Preview.Width-1,FLayer.Preview.Height-1);
    FLayer.Preview.Canvas.Pen.Width:=2;
    FLayer.Preview.Canvas.Brush.Style:=bsSolid;
    FLayer.Preview.Canvas.Brush.Color:=LayerFontColors[integer(FLayer.Layer.LayerType)];
    FLayer.Preview.Canvas.FrameRect(ARect);

    FLayer.Preview.Canvas.PenPos:=Point(0,FLayer.Preview.Height);
    FLayer.Preview.Canvas.Pen.Color:=RGB(90,90,90);
    FLayer.Preview.Canvas.LineTo(0,0);
    FLayer.Preview.Canvas.LineTo(FLayer.Preview.Width,0);
    FLayer.Preview.Canvas.Pen.Color:=RGB(225,225,225);
    FLayer.Preview.Canvas.LineTo(FLayer.Preview.Width,FLayer.Preview.Height);
    FLayer.Preview.Canvas.LineTo(0,FLayer.Preview.Height);
  end
  else
  begin
    FLayer.Preview.Canvas.PenPos:=Point(0,FLayer.Preview.Height);
    FLayer.Preview.Canvas.Pen.Color:=RGB(225,225,225);
    FLayer.Preview.Canvas.LineTo(0,0);
    FLayer.Preview.Canvas.LineTo(FLayer.Preview.Width,0);
    FLayer.Preview.Canvas.Pen.Color:=RGB(90,90,90);
    FLayer.Preview.Canvas.LineTo(FLayer.Preview.Width,FLayer.Preview.Height);
    FLayer.Preview.Canvas.LineTo(0,FLayer.Preview.Height);
  end;

  if FLayer.IsScaled then
  begin
    FLayer.Preview.Canvas.Pen.Width:=1;
    FLayer.Preview.Canvas.Brush.Style:=bsSolid;

    ArrowPos:=FLayer.Preview.Width div 4;
    while (ArrowPos < FLayer.Preview.Width - 4) do
    begin
      FLayer.Preview.Canvas.Pen.Color:=clGray;
      FLayer.Preview.Canvas.Brush.Color:=clBlack;
      DrawArrow(FLayer.Preview.Canvas,ArrowPos + 4,FLayer.Preview.Height - 50,10);

      FLayer.Preview.Canvas.Pen.Color:=clLime;
      FLayer.Preview.Canvas.Brush.Color:=clGreen;
      DrawArrow(FLayer.Preview.Canvas,ArrowPos,FLayer.Preview.Height - 53,10);

      ArrowPos:=ArrowPos + FLayer.Preview.Width div 4;
    end;
  end;

end;

procedure TfrmMain.pbLayerPreviewClick(Sender: TObject);
var
  FLayer:PLayerWithPreview;
begin
  FLayer:=Pointer((Sender as TPaintBox).Tag);
  ActiveLayer:=FLayer;
end;


procedure TfrmMain.SetActiveLayer(const Value: PLayerWithPreview);
var
  OldValue:PLayerWithPreview;
  Index:integer;
  I:integer;
begin
//  Exit;

  OldValue:=FActiveLayer;
  FActiveLayer:=nil;
  if Assigned(OldValue) then
    OldValue.Preview.Invalidate;

  FActiveLayer := Value;

  if not Assigned(Value) then
  begin
    mnuProjectDeleteLayer.Enabled:=false;
    mnuProjectMoveLayerUp.Enabled:=false;
    mnuProjectMoveLayerDown.Enabled:=false;
    mnuProjectExportLayer.Enabled:=false;
    lblDeleteLayer.Enabled:=false;
    lblMoveLayerUp.Enabled:=false;
    lblMoveLayerDown.Enabled:=false;
    dxLayerInspector.Enabled:=false;
    SetInspectorRows(nil);
    Exit;
  end;
  FActiveLayer.Preview.Invalidate;
  dxLayerInspector.Enabled:=true;
  lblDeleteLayer.Enabled:=true;
  Index:=LayerList.IndexOf(FActiveLayer);
  if Index<2 then
    lblMoveLayerUp.Enabled:=false
  else
    lblMoveLayerUp.Enabled:=true;

  if Index>LayerList.Count-2 then
    lblMoveLayerDown.Enabled:=false
  else
  begin
    OldValue:=LayerList.Items[Index + 1];
    if OldValue.Layer.LayerType=ltSubgrade then
      lblMoveLayerDown.Enabled:=false
    else
      lblMoveLayerDown.Enabled:=true;
  end;
  mnuProjectDeleteLayer.Enabled:=true;
  mnuProjectExportLayer.Enabled:=true;

  case FActiveLayer.Layer.LayerType of
    ltAC:
    begin
      lblMoveLayerUp.Enabled:=false;
      lblMoveLayerDown.Enabled:=false;
      dxLayerStyle.ReadOnly:=true;
      dxLayerModel.ReadOnly:=true;
    end;
    ltSubgrade:
    begin
      lblMoveLayerUp.Enabled:=false;
      lblMoveLayerDown.Enabled:=false;
      dxLayerStyle.ReadOnly:=false;
      dxLayerModel.ReadOnly:=false;
    end;
    else
    begin
      dxLayerStyle.ReadOnly:=false;
      dxLayerModel.ReadOnly:=false;
    end;
  end;
  mnuProjectMoveLayerUp.Enabled:=lblMoveLayerUp.Enabled;
  mnuProjectMoveLayerDown.Enabled:=lblMoveLayerDown.Enabled;

  SetRowTags(FActiveLayer);

  //General Properties
  for I:=0 to dxLayerInspector.TotalRowCount-1 do
  begin
    case I of
      1:dxLayerType.Text:=dxLayerType.Items.Strings[integer(FActiveLayer.Layer.LayerType)];
      3:if FActiveLayer.Layer.IsIsotropic then
          dxLayerStyle.Text:='Isotropic'
        else
          dxLayerStyle.Text:='Anisotropic';
      4:
      begin
        if FActiveLayer.Layer.LayerType=ltSubgrade then
        begin
          if FActiveLayer.Layer.IsKThetaModel then
            dxLayerModel.Text:='Bilinear Model'
          else
            dxLayerModel.Text:='Brown _Loach Model';
        end
        else
        begin
          if FActiveLayer.Layer.IsKThetaModel then
            dxLayerModel.Text:='K-Theta Model'
          else
            dxLayerModel.Text:='Uzan Model';
        end;
      end;
      else SetValueFromTag(I);
    end;
  end;
  SetInspectorRows(FActiveLayer);
  for I:=0 to dxLayerInspector.LastNode.Index do
  begin
    dxLayerInspector.Items[I].Expand(true);
  end;

  Exit;

//  dxLayerType.Text:= ;
//  dxLayerThickness.Text:=FloatToStr(FActiveLayer.Layer.Thickness);

  //IsoTropic Properties
  dxIsoModulus.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.Modulus);
  dxIsoPoisson.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.Poissons_Ratio);
  dxIsoBodyForce.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.BodyForce);
  dxIsoCohesion.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.Cohesion);
  dxIsoRest.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.RestStress);
  dxIsoFriction.Text:=FloatToStr(FActiveLayer.Layer.Isotropic.InternalFriction);

  //AnisoTropic Properties
  dxAnisoVertModulus.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.ModulusVertical);
  dxAnisoHorzPoisson.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.PoissonHorizontal);
  dxAnisoVertPoisson.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.PoissonVertical);
  dxAnisoHorzToVert.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.HorzToVertModular);
  dxAnisoBodyforce.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.BodyForce);
  dxAnisoFriction.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.InternalFriction);
  dxAnisoCohesion.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.Cohesion);
  dxAnisoRest.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.RestStress);
  dxAnisoShearToVert.Text:=FloatToStr(FActiveLayer.Layer.Anisotropic.ShearToVertModular);

  //KTheta Properties
  dxKThetaKV1.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K1V);
  dxKThetaKV2.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K2V);
  dxKThetaKH1.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K1H);
  dxKThetaKH2.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K2H);
  dxKThetaKS1.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K1S);
  dxKThetaKS2.Text:=FloatToStr(FActiveLayer.Layer.KThetaModel.K2S);

  //Uzan Properties
  dxUzanV1.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K1V);
  dxUzanV2.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K2V);
  dxUzanV3.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K3V);
  dxUzanH1.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K1H);
  dxUzanH2.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K2H);
  dxUzanH3.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K3H);
  dxUzanS1.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K1S);
  dxUzanS2.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K2S);
  dxUzanS3.Text:=FloattoStr(FActiveLayer.Layer.UzanModel.K3S);


  //Bilinear Model
  dxBilinearK1.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.K1);
  dxBilinearK2.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.K2);
  dxBilinearK3.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.K3);
  dxBilinearK4.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.K4);
  dxBilinearDevlow.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.Devlow);
  dxBilinearDevHigh.Text:=FloatToStr(FActiveLayer.Layer.BilinearModel.Devhigh);

  //Brown _Loach Model
  dxBrownC.Text:=FloatToStr(FActiveLayer.Layer.BrownModel.C);
  dxBrownD.Text:=FloatToStr(FActiveLayer.Layer.BrownModel.D);
end;

procedure TfrmMain.gbLayerPreviewClick(Sender: TObject);
begin
  ActiveLayer:=nil;
end;

procedure TfrmMain.mnuInputWheelClick(Sender: TObject);
begin
  if PageControl.ActivePage=tsWheelLoad then Exit;
  cmdRestoreWheel.Click;

  AllowPageChange:=true;
  PageControl.ActivePage:=tsWheelLoad;
  AllowPageChange:=false;
end;

procedure TfrmMain.mnuProjectLayersClick(Sender: TObject);
begin
  AllowPageChange:=true;
  PageControl.ActivePage:=tbLayers;
  AllowPageChange:=false;
end;

procedure TfrmMain.pnlCaption1Resize(Sender: TObject);
begin
  ImgGradient1.Width:=(Sender as TPanel).Width;
  ImgGradient2.Width:=(Sender as TPanel).Width;
  ImgGradient3.Width:=(Sender as TPanel).Width;
  ImgGradient4.Width:=(Sender as TPanel).Width;
  ImgGradient5.Width:=(Sender as TPanel).Width;

  ImgGradient1.Left:=0;
  ImgGradient2.Left:=0;
  ImgGradient3.Left:=0;
  ImgGradient4.Left:=0;
  ImgGradient5.Left:=0;

  ImgLogo1.Left:=(Sender as TPanel).Width - ImgLogo1.Width;
  ImgLogo2.Left:=(Sender as TPanel).Width - ImgLogo2.Width;
  ImgLogo3.Left:=(Sender as TPanel).Width - ImgLogo3.Width;
  ImgLogo4.Left:=(Sender as TPanel).Width - ImgLogo4.Width;
  ImgLogo5.Left:=(Sender as TPanel).Width - ImgLogo4.Width;

  lblLayerAndMaterials.Width:=ImgLogo1.Left;
  lblExecutionDetails.Width:=ImgLogo1.Left;
  lblWelcome.Width:=ImgLogo1.Left;
  lblWheelLoad.Width:=ImgLogo1.Left;
end;

procedure TfrmMain.mnuFileExitClick(Sender: TObject);
begin
  Close;
end;

procedure TfrmMain.mnuFileNewProjectClick(Sender: TObject);
begin
  if CloseProject(true) then
  begin
    ClearHistory;
    StartNewProject;
  end;

end;

procedure TfrmMain.SetInspectorRows(FLayer: PLayerWithPreview);
var
  I:integer;
  AStream:TMemoryStream;
  szFileName:string;
  ActiveNode:TdxInspectorNode;
begin
  dxLayerInspector.BeginUpdate;

  for I:=0 to dxLayerInspector.TotalRowCount-1 do
    dxLayerInspector.Rows[I].Visible:=false;


  if not Assigned(FLayer) then
  begin
    dxLayerInspector.EndUpdate;
    Exit;
  end;

  case FLayer.Layer.LayerType of
    ltAC:
    begin
      szFileName:='AC';
    end;
    else
    begin
      if FLayer.Layer.IsIsotropic then
        szFileName:='Iso'
      else
        szFileName:='Aniso';
      if FLayer.Layer.IsKThetaModel then
      begin
        if FLayer.Layer.LayerType=ltSubgrade then
          szFileName:=szFileName + 'Bilinear'
        else
          szFileName:=szFileName + 'KTheta';
      end
      else
      begin
        if FLayer.Layer.LayerType=ltSubgrade then
          szFileName:=szFileName + 'Brown'
        else
          szFileName:=szFileName + 'Uzan';
      end;
    end;
  end;
  szFileName:=ExtractFilePath(ParamStr(0)) + szFileName + '.lip';

  ActiveNode:=dxLayerInspector.FocusedNode;

  AStream:=TMemoryStream.Create;
  AStream.LoadFromFile(szFileName);
  AStream.Position:=0;
  dxLayerInspector.LoadNodesFromStream(AStream);
  AStream.Free;

  for I:=dxLayerInspector.LastNode.Index downto 0 do
  begin
    if not dxLayerInspector.Items[I].HasChildren then
      (dxLayerInspector.Items[I] as TdxInspectorRowNode).Row.Visible:=false;
  end;

  for I:=dxLayerInspector.LastNode.Index downto 0 do
  begin
    if dxLayerInspector.Items[I]=ActiveNode then
    begin
      dxLayerInspector.Items[I].Focused:=true;
      break;
    end;
  end;

  dxLayerType.ReadOnly:=true;
  if FLayer.Layer.LayerType=ltAC then
  begin
    dxLayerStyle.ReadOnly:=true;
    dxLayerModel.Visible:=false;
  end;

  dxLayerInspector.EndUpdate;

end;

procedure TfrmMain.mnuProjectExecutionDetailsClick(Sender: TObject);
begin
  if PageControl.ActivePage=tbExecutionDetails then Exit;
  AllowPageChange:=true;
  cmdRestoreExecution.Click;
  PageControl.ActivePage:=tbExecutionDetails;
  AllowPageChange:=false;
end;

procedure TfrmMain.PageControlPageChanging(Sender: TObject;
  NewPage: TcxTabSheet; var AllowChange: Boolean);
begin
  if cmdApplyWheel.Enabled then
  begin
    case MessageBox(Handle,'Your changes are not saved. Do you want to save your changes?',PChar(Application.Title),MB_YESNOCANCEL + MB_ICONQUESTION) of
      ID_YES:cmdApplyWheelClick(cmdApplyWheel);
      ID_CANCEL:
      begin
        AllowChange:=false;
        Exit;
      end;
    end;
  end;
  if cmdApplyExecution.Enabled then
  begin
    case MessageBox(Handle,'Your changes are not saved. Do you want to save your changes?',PChar(Application.Title),MB_YESNOCANCEL + MB_ICONQUESTION) of
      ID_YES:cmdApplyExecutionClick(cmdApplyExecution);
      ID_CANCEL:
      begin
        AllowChange:=false;
        Exit;
      end;
    end;
  end;

  AllowChange:=AllowPageChange;
end;

procedure TfrmMain.mnuFileOpenClick(Sender: TObject);
begin
  if CloseProject(true) then
  begin
    if dlgOpen.Execute then
      OpenProjectFromFile(dlgOpen.FileName);
  end;
end;

procedure TfrmMain.SetModified(const Value: boolean);
begin
  FModified := Value;
  ProjectFileName:=ProjectFileName;
end;

procedure TfrmMain.mnuHelpHelpClick(Sender: TObject);
begin
  ShowMessage('No Help');
end;

procedure TfrmMain.pnlLayerPreviewBGMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  ActiveLayer:=nil;
end;

procedure TfrmMain.txtRadiusPropertiesChange(Sender: TObject);
begin
  if (Sender as TcxSpinEdit).Focused then
  begin
    txtLoad.Value:=WheelData_From_Contact;
    cmdApplyWheel.Enabled:=true;
    cmdRestoreWheel.Enabled:=true;
  end;
end;

procedure TfrmMain.txtContactPropertiesChange(Sender: TObject);
begin
  if (Sender as TcxSpinEdit).Focused then
  begin
    txtLoad.Value:=WheelData_From_Contact;
    cmdApplyWheel.Enabled:=true;
    cmdRestoreWheel.Enabled:=true;
  end;
end;

procedure TfrmMain.txtLoadPropertiesChange(Sender: TObject);
begin
  if (Sender as TcxSpinEdit).Focused then
  begin
    txtContact.Value:=Contact_From_WheelData;
    cmdApplyWheel.Enabled:=true;
    cmdRestoreWheel.Enabled:=true;
  end;
end;

function TfrmMain.Contact_From_WheelData: double;
var
  Radius:double;
  WheelLoad:double;
  tmp:double;
begin
  Radius := txtRadius.Value;
  WheelLoad:=txtLoad.Value;

  tmp:=pi * (Radius * Radius);
  Result:=RoundTo(WheelLoad / tmp ,-1);

end;

function TfrmMain.WheelData_From_Contact: double;
var
  Radius:double;
  ContactPressure:double;
  tmp:double;
begin
  Radius := txtRadius.Value;
  ContactPressure:=txtContact.Value;
  tmp:=pi * (Radius * Radius);
  Result:=RoundTo(tmp * ContactPressure,-1);
end;

procedure TfrmMain.cmdRestoreWheelClick(Sender: TObject);
begin
  txtRadius.Value:=WheelInfo.Radius;
  txtContact.Value:=WheelInfo.ContactPressure;
  txtLoad.Value:=WheelInfo.WheelLoad;

  cmdApplyWheel.Enabled:=false;
  cmdRestoreWheel.Enabled:=false;
end;

procedure TfrmMain.cmdApplyWheelClick(Sender: TObject);
begin
  AddToHistory;

  WheelInfo.Radius:=txtRadius.Value;
  WheelInfo.ContactPressure:=txtContact.Value;
  WheelInfo.WheelLoad:=txtLoad.Value;

  cmdApplyWheel.Enabled:=false;
  cmdRestoreWheel.Enabled:=false;

  Modified:=true;
end;

procedure TfrmMain.PageControlChange(Sender: TObject);
begin
  if PageControl.ActivePageIndex=0 then
  begin
    mnuZoomIn.Enabled:=true;
    mnuZoomOut.Enabled:=true;
  end
  else
  begin
    mnuZoomIn.Enabled:=false;
    mnuZoomOut.Enabled:=false;
  end;
  case PageControl.ActivePageIndex of
    0:sbStatus.Panels[ActionPanelID].Text:='Layer & Material Properties';
    1:sbStatus.Panels[ActionPanelID].Text:='Startup Screen';
    2:sbStatus.Panels[ActionPanelID].Text:='Execution Details';
    3:sbStatus.Panels[ActionPanelID].Text:='Wheel Load Information';
  end;
end;

procedure TfrmMain.cmdApplyExecutionClick(Sender: TObject);
begin
  AddToHistory;

  StrPCopy(ProjectInfo.Title,txtAnalyseTitle.Text);
  StrPCopy(ProjectInfo.Creator,txtCreatorName.Text);
  StrPCopy(ProjectInfo.Name,txtProjectName.Text);
  StrPCopy(ProjectInfo.Description,txtProjectDescription.Text);

  cmdApplyExecution.Enabled:=false;
  cmdRestoreExecution.Enabled:=false;

  Modified:=true;
end;

procedure TfrmMain.cmdRestoreExecutionClick(Sender: TObject);
begin
  txtAnalyseTitle.Text:=ProjectInfo.Title;
  txtCreatorName.Text:=ProjectInfo.Creator;
  txtProjectName.Text:=ProjectInfo.Name;
  txtProjectDescription.Text:=ProjectInfo.Description;

  cmdApplyExecution.Enabled:=false;
  cmdRestoreExecution.Enabled:=false;
end;

procedure TfrmMain.txtAnalyseTitlePropertiesChange(Sender: TObject);
begin
  if(Sender as TcxCustomEdit).Focused then
  begin
    cmdApplyExecution.Enabled:=true;
    cmdRestoreExecution.Enabled:=true;
  end;
end;

procedure TfrmMain.mnuViewMainToolbarClick(Sender: TObject);
begin
  dxBarManager.Bars[1].Visible:=mnuViewMainToolbar.Down;
end;

procedure TfrmMain.dxBarManagerBarVisibleChange(Sender: TdxBarManager;
  ABar: TdxBar);
begin
  case ABar.Index of
    1:mnuViewMainToolbar.Down:=ABar.Visible;
  end;
end;

procedure TfrmMain.mnuViewStatusBarClick(Sender: TObject);
begin
  sbStatus.Visible:=mnuViewStatusBar.Down;
end;

procedure TfrmMain.mnuViewNavBarClick(Sender: TObject);
begin
  nbNavigator.Visible:=mnuViewNavBar.Down;
  MainSplitter.Visible:=mnuViewNavBar.Down;
  MainSplitter.Left:=nbNavigator.Width;
end;

function TfrmMain.CloseProject(DoSave: boolean): boolean;
var
  szFileName:string;
  I:integer;
  FLayer:PLayerWithPreview;
begin
  Result:=false;
  if Modified then
  begin
    case MessageBox(Handle,'Do you want to save your changes?',PChar(Application.Title),MB_YESNOCANCEL + MB_ICONQUESTION) of
      ID_YES:
      begin
        szFileName:=ProjectFileName;
        if FileExists(szFileName)=false then
        begin
          szFileName:=GetFileName(dlgSave,'.pro');
          if szFileName='' then Exit;
        end;
        SaveProjectToFile(szFileName);
      end;
      ID_CANCEL: Exit;
    end;
  end;

  //Layerleri temizle
  ActiveLayer:=nil;
  for I:=0 to LayerList.Count-1 do
  begin
    FLayer:=LayerList.Items[I];
    FLayer.Preview.Free;
    Dispose(FLayer);
  end;
  LayerList.Clear;


  ProjectFileName:='';
  Modified:=false;

  Result:=true;
end;

procedure TfrmMain.mnuFileCloseClick(Sender: TObject);
begin
  CloseProject(true);

  //Undo Buffer�n� temizle
  ClearHistory;

  //Arabirimi d�zenle
  nbNavigator.Visible:=false;
  MainSplitter.Visible:=false;
  PageControl.Visible:=false;

end;

procedure TfrmMain.mnuFileSaveClick(Sender: TObject);
var
  szFileName:string;
begin
  if ProjectFileName='' then
  begin
    szFileName:=GetFileName(dlgSave,'.pro');
    if szFileName='' then Exit;
    ProjectFileName:=szFileName;
  end;
  SaveProjectToFile(ProjectFileName);
end;

procedure TfrmMain.mnuFileSaveAsClick(Sender: TObject);
var
  szFileName:string;
begin
  szFileName:=GetFileName(dlgSave,'.pro');
  if szFileName='' then Exit;
  ProjectFileName:=szFileName;
  SaveProjectToFile(ProjectFileName);
end;

procedure TfrmMain.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  CanClose:=CloseProject(true);
end;

procedure TfrmMain.mnuHelpAboutClick(Sender: TObject);
begin
  ShowMessage('About penceresi gelecek');
end;

procedure TfrmMain.SetRowTags(FLayer: PLayerWithPreview);
begin

  //General Properties


  dxLayerType.Tag:=Integer(Addr(FLayer.Layer.LayerType));
  dxLayerThickness.Tag:=Integer(Addr(FLayer.Layer.Thickness));
  dxLayerStyle.Tag:=Integer(Addr(FLayer.Layer.IsIsotropic));
  dxLayerModel.Tag:=Integer(Addr(FLayer.Layer.IsKThetaModel));

  //IsoTropic Properties
  dxIsoModulus.Tag:=Integer(Addr(FLayer.Layer.Isotropic.Modulus));
  dxIsoPoisson.Tag:=Integer(Addr(FLayer.Layer.Isotropic.Poissons_Ratio));
  dxIsoBodyForce.Tag:=Integer(Addr(FLayer.Layer.Isotropic.BodyForce));
  dxIsoCohesion.Tag:=Integer(Addr(FLayer.Layer.Isotropic.Cohesion));
  dxIsoRest.Tag:=Integer(Addr(FLayer.Layer.Isotropic.RestStress));
  dxIsoFriction.Tag:=Integer(Addr(FLayer.Layer.Isotropic.InternalFriction));

  //AnisoTropic Properties
  dxAnisoVertModulus.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.ModulusVertical));
  dxAnisoHorzPoisson.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.PoissonHorizontal));
  dxAnisoVertPoisson.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.PoissonVertical));
  dxAnisoHorzToVert.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.HorzToVertModular));
  dxAnisoBodyforce.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.BodyForce));
  dxAnisoFriction.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.InternalFriction));
  dxAnisoCohesion.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.Cohesion));
  dxAnisoRest.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.RestStress));
  dxAnisoShearToVert.Tag:=Integer(Addr(FLayer.Layer.Anisotropic.ShearToVertModular));

  //KTheta Properties
  dxKThetaKV1.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K1V));
  dxKThetaKV2.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K2V));
  dxKThetaKH1.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K1H));
  dxKThetaKH2.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K2H));
  dxKThetaKS1.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K1S));
  dxKThetaKS2.Tag:=Integer(Addr(FLayer.Layer.KThetaModel.K2S));

  //Uzan Properties
  dxUzanV1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1V));
  dxUzanV2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2V));
  dxUzanV3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3V));
  dxUzanH1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1H));
  dxUzanH2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2H));
  dxUzanH3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3H));
  dxUzanS1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1S));
  dxUzanS2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2S));
  dxUzanS3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3S));


  //Uzan Properties
  dxUzanV1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1V));
  dxUzanV2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2V));
  dxUzanV3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3V));
  dxUzanH1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1H));
  dxUzanH2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2H));
  dxUzanH3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3H));
  dxUzanS1.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K1S));
  dxUzanS2.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K2S));
  dxUzanS3.Tag:=Integer(Addr(FLayer.Layer.UzanModel.K3S));

  dxBilinearK1.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.K1));
  dxBilinearK2.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.K2));
  dxBilinearK3.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.K3));
  dxBilinearK4.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.K4));
  dxBilinearDevlow.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.DevLow ));
  dxBilinearDevhigh.Tag:=Integer(Addr(FLayer.Layer.BilinearModel.DevHigh ));

  dxBrownC.Tag:=Integer(Addr(FLayer.Layer.BrownModel.C));
  dxBrownD.Tag:=Integer(Addr(FLayer.Layer.BrownModel.D));
end;

procedure TfrmMain.SetValueFromTag(Row: integer);
var
  tmp:PDouble;
begin
  if dxLayerInspector.Rows[Row].Tag=0 then exit;

  tmp:=Pointer(dxLayerInspector.Rows[Row].Tag);
  (dxLayerInspector.Rows[Row] as TdxInspectorTextSpinRow).Text:=FloatToStr(tmp^);
//  tmp^:=StrToFloat((dxLayerInspector.Rows[Row] as TdxInspectorTextSpinRow).Text);
end;

procedure TfrmMain.dxLayerInspectorEdited(Sender: TObject;
  Node: TdxInspectorNode; Row: TdxInspectorRow);
var
  tmp:PDouble;
  NewValue:double;
begin
  if Row.InheritsFrom(TdxInspectorPickRow) then
    Exit
  else
  begin
    AddToHistory;
    tmp:=Pointer(Row.Tag);
    if TryStrToFloat(Row.DisplayText,NewValue) then
      tmp^:=NewValue
    else
      (Row as TdxInspectorTextSpinRow).Text:=FloatToStr(tmp^);

    Modified:=true;
    ScaleLayerPreviews;
  end;
end;

procedure TfrmMain.dxLayerStyleChange(Sender: TObject);
var
  NewValue:string;
  Index:integer;
  LayerID,RowID:integer;
begin
  AddToHistory;

  NewValue:=TdxInplaceInspectorEdit(dxLayerInspector.InplaceEditor).Text;
  Index:=dxLayerStyle.Items.IndexOf(NewValue);
  if Index=0 then
    ActiveLayer.Layer.IsIsotropic:=true
  else
    ActiveLayer.Layer.IsIsotropic:=false;

  Modified:=true;

  LayerID:=LayerList.IndexOf(ActiveLayer);
  RowID:=dxLayerInspector.IndexOfRow(dxLayerStyle);
  PostMessage(Handle,WM_DISPLAYLAYER,LayerID,RowID);
end;

procedure TfrmMain.WmDisplayLayer(var Msg: TMessage);
begin
  ActiveLayer:=LayerList.Items[Msg.WParam];
  dxLayerInspector.Rows[Msg.LParam].Node.Focused:=true;

end;

procedure TfrmMain.dxLayerModelChange(Sender: TObject);
var
  NewValue:string;
  Index:integer;
  LayerID,RowID:integer;
begin
  AddToHistory;

  NewValue:=TdxInplaceInspectorEdit(dxLayerInspector.InplaceEditor).Text;
  Index:=dxLayerModel.Items.IndexOf(NewValue);
  if Index=0 then
    ActiveLayer.Layer.IsKThetaModel:=true
  else
    ActiveLayer.Layer.IsKThetaModel:=false;
  Modified:=true;

  LayerID:=LayerList.IndexOf(ActiveLayer);
  RowID:=dxLayerInspector.IndexOfRow(dxLayerModel);

  PostMessage(Handle,WM_DISPLAYLAYER,LayerID,RowID);
end;

procedure TfrmMain.mnuProjectLayerWizardClick(Sender: TObject);
var
  FNewLayer:TfrmNewLayer;
  FLayer:PLayerWithPreview;
  I:integer;
  tmp:PLayerWithPreview;
begin
  FNewLayer:=TfrmNewLayer.Create(self);

  for I:=0 to LayerList.Count-1 do
  begin
    FLayer:=LayerList.Items[I];
    if FLayer.Layer.LayerType=ltAC then
      FNewLayer.rbAC.Enabled:=false;
    if FLayer.Layer.LayerType=ltSubgrade then
      FNewLayer.rbSubgrade.Enabled:=false;
  end;

  FNewLayer.ShowModal;

  if FNewLayer.Saving then
  begin
    AddToHistory;

    New(FLayer);
    if FNewLayer.rbAC.Checked then FLayer.Layer.LayerType:=ltAC;
    if FNewLayer.rbBase.Checked then FLayer.Layer.LayerType:=ltBase;
    if FNewLayer.rbSubbase.Checked then FLayer.Layer.LayerType:=ltSubbase;
    if FNewLayer.rbSubgrade.Checked then FLayer.Layer.LayerType:=ltSubgrade;

    FLayer.Layer.Thickness:=FNewLayer.txtLayerThickness.Value;

    FLayer.Layer.IsIsotropic:=FNewLayer.rbIsotropic.Checked;

    //Isotropic
    FLayer.Layer.Isotropic.Modulus:=FNewLayer.txtIsoModulus.Value;
    FLayer.Layer.Isotropic.Poissons_Ratio:=FNewLayer.txtIsoPoisson.Value;
    FLayer.Layer.Isotropic.BodyForce:=FNewLayer.txtIsoBodyForce.Value;
    FLayer.Layer.Isotropic.InternalFriction:=FNewLayer.txtIsoFriction.Value;
    FLayer.Layer.Isotropic.Cohesion:=FNewLayer.txtIsoCohesion.Value;
    FLayer.Layer.Isotropic.RestStress:=FNewLayer.txtIsoRest.Value;

    //Anisotropic
    FLayer.Layer.Anisotropic.ModulusVertical:=FNewLayer.txtAnisoModulus.Value;
    FLayer.Layer.Anisotropic.PoissonHorizontal:=FNewLayer.txtAnisoHorzPoisson.Value;
    FLayer.Layer.Anisotropic.PoissonVertical:=FNewLayer.txtAnisoVertPoisson.Value;
    FLayer.Layer.Anisotropic.HorzToVertModular:=FNewLayer.txtAnisoHVRatio.Value;
    FLayer.Layer.Anisotropic.ShearToVertModular:=FNewLayer.txtAnisoSVRatio.Value;
    FLayer.Layer.Anisotropic.BodyForce:=FNewLayer.txtAnisoBodyForce.Value;
    FLayer.Layer.Anisotropic.InternalFriction:=FNewLayer.txtAnisoFriction.Value;
    FLayer.Layer.Anisotropic.Cohesion:=FNewLayer.txtAnisoCohesion.Value;
    FLayer.Layer.Anisotropic.RestStress:=FNewLayer.txtAnisoRest.Value;

    FLayer.Layer.IsKThetaModel:=FNewLayer.rbKTheta.Checked;

    //K-Theta Model
    FLayer.Layer.KThetaModel.K1V:=FNewLayer.txtKThetaV1.Value;
    FLayer.Layer.KThetaModel.K2V:=FNewLayer.txtKThetaV2.Value;
    FLayer.Layer.KThetaModel.K1H:=FNewLayer.txtKThetaH1.Value;
    FLayer.Layer.KThetaModel.K2H:=FNewLayer.txtKThetaH2.Value;
    FLayer.Layer.KThetaModel.K1S:=FNewLayer.txtKThetaS1.Value;
    FLayer.Layer.KThetaModel.K2S:=FNewLayer.txtKThetaS2.Value;

    //Uzan Model
    FLayer.Layer.UzanModel.K1V:=FNewLayer.txtUzanV1.Value;
    FLayer.Layer.UzanModel.K2V:=FNewLayer.txtUzanV2.Value;
    FLayer.Layer.UzanModel.K3V:=FNewLayer.txtUzanV3.Value;
    FLayer.Layer.UzanModel.K1H:=FNewLayer.txtUzanH1.Value;
    FLayer.Layer.UzanModel.K2H:=FNewLayer.txtUzanH2.Value;
    FLayer.Layer.UzanModel.K3H:=FNewLayer.txtUzanH3.Value;
    FLayer.Layer.UzanModel.K1S:=FNewLayer.txtUzanS1.Value;
    FLayer.Layer.UzanModel.K2S:=FNewLayer.txtUzanS2.Value;
    FLayer.Layer.UzanModel.K3S:=FNewLayer.txtUzanS3.Value;

    case FLayer.Layer.LayerType of
      ltAC:FLayer.Caption:='ASPHALT CONCRETE';
      ltBase:FLayer.Caption:='BASE LAYER';
      ltSubbase:FLayer.Caption:='SUBBASE LAYER';
      ltSubgrade:FLayer.Caption:='SUBGRADE LAYER';
    end;

    CreateLayerPreviewComponent(FLayer);

    if FLayer.Layer.LayerType=ltAC then
    begin
      LayerList.Insert(0,FLayer);
      FLayer.Preview.Top:=0;
    end
    else
    begin
      LayerList.Add(FLayer);
      if LayerList.Count>=2 then
      begin
        tmp:=LayerList.Items[LayerList.Count-2];
        if tmp.Layer.LayerType=ltSubgrade then
        begin
          LayerList.Exchange(LayerList.Count-2,LayerList.Count-1);
          tmp.Preview.Top:=FLayer.Preview.Top + FLayer.Preview.Height;
        end;
      end;
    end;
    ScaleLayerPreviews;
    CheckLayerHeights;

    Modified:=true;
  end;

  FNewLayer.Free;
end;

procedure TfrmMain.mnuProjectMoveLayerUpClick(Sender: TObject);
var
  Index:integer;
  FLayer:PLayerWithPreview;
begin
  AddToHistory;

  Index:=LayerList.IndexOf(ActiveLayer);

  LayerList.Move(Index,Index-1);
  FLayer:=LayerList.Items[Index-2];
  ActiveLayer.Preview.Top:=FLayer.Preview.Top + FLayer.Preview.Height;
  ActiveLayer:=ActiveLayer;

  Modified:=true;

end;

procedure TfrmMain.mnuProjectMoveLayerDownClick(Sender: TObject);
var
  Index:integer;
  FLayer:PLayerWithPreview;
begin
  AddToHistory;

  Index:=LayerList.IndexOf(ActiveLayer);

  LayerList.Move(Index,Index+1);
  FLayer:=LayerList.Items[Index];
  ActiveLayer.Preview.Top:=FLayer.Preview.Top + FLayer.Preview.Height;
  ActiveLayer:=ActiveLayer;

  Modified:=true;

end;

procedure TfrmMain.mnuProjectDeleteLayerClick(Sender: TObject);
var
  msg:string;
  FLayer:PLayerWithPreview;
begin
  msg:='Do you really want to delete selected ' + ActiveLayer.Caption + '?';
  if MessageBox(Handle,PChar(msg),PChar(Application.Title),MB_YESNO + MB_ICONQUESTION)=ID_YES then
  begin
    AddToHistory;

    FLayer:=ActiveLayer;
    ActiveLayer:=nil;

    LayerList.Delete(LayerList.IndexOf(FLayer));
    FLayer.Preview.Free;
    Dispose(FLayer);

    ScaleLayerPreviews;
    Modified:=true;
  end;
end;

procedure TfrmMain.dxLayerInspectorDrawCaption(Sender: TdxInspectorRow;
  ACanvas: TCanvas; ARect: TRect; var AText: String; AFont: TFont;
  var AColor: TColor; var ADone: Boolean);
begin
  if Sender.Node.Count>0 then
    AColor:=dxLayerInspector.GridColor
  else
    AColor:=RGB( (GetRValue(dxLayerInspector.Color) + GetRValue(dxLayerInspector.GridColor)) div 2,
                 (GetGValue(dxLayerInspector.Color) + GetGValue(dxLayerInspector.GridColor)) div 2,
                 (GetBValue(dxLayerInspector.Color) + GetBValue(dxLayerInspector.GridColor)) div 2);

end;

function TfrmMain.GetFileName(SaveDialog: TSaveDialog;Ext:string): string;
var
  szFileName:string;
begin
  Result:='';
  if SaveDialog.Execute then
  begin
    szFileName:=ChangeFileExt(SaveDialog.FileName,Ext);
    while (FileExists(szFileName)) do
    begin
      case MessageBox(Handle,PChar('File listed below already exists.' + #10#13#10#13 + szFileName + #10#13#10#13 + 'Do you want to overwrite?'),PChar(Application.Title),MB_YESNOCANCEL + MB_ICONQUESTION) of
        ID_YES:
        begin
          Result:=szFileName;
          Exit;
        end;
        ID_CANCEL:
        begin
          Result:='';
          Exit;
        end;
        ID_NO:
        begin
          if not SaveDialog.Execute then
          begin
            Result:='';
            Exit;
          end;
          szFileName:=ChangeFileExt(SaveDialog.FileName,Ext);
        end;
      end;
    end;
    Result:=szFileName;
  end;
end;

procedure TfrmMain.mnuProjectExportLayerClick(Sender: TObject);
var
  tmp:TMemoryStream;
  szFileName:string;
  Sign:Array [0..4] of char;
  Version:double;
begin
  szFileName:=GetFileName(dlgExportLayer,'.lif');
  if szFileName<>'' then
  begin
    StrPCopy(Sign,LayerSign);
    Version:=LayerFileVersion;

    tmp:=TMemoryStream.Create;
    tmp.Write(Sign,High(Sign));
    tmp.Write(Version,sizeof(Version));
    tmp.Write(ActiveLayer.Layer,sizeof(ActiveLayer.Layer));
    tmp.Position:=0;
    tmp.SaveToFile(szFileName);
    tmp.Free;;
  end;
end;

procedure TfrmMain.mnuProjectImportLayerClick(Sender: TObject);
var
  szFileName:string;
begin
  if dlgImportLayer.Execute then
  begin

    szFileName:=ChangeFileExt(dlgImportLayer.FileName,'.lif');
    if not FileExists(szFileName) then
    begin
      MessageBox(Handle,PChar('File listed below not found.' + #10#13#10#13 + szFileName),PChar(Application.Title),MB_OK + MB_ICONINFORMATION);
      Exit;
    end;
//    AddToHistory;
    AddLayerFromFile(szFileName);
    CheckLayerHeights;
    ScaleLayerPreviews;
    Modified:=true;
  end;
end;

procedure TfrmMain.CreateLayerPreviewComponent(FLayer: PLayerWithPreview);
var
  tmp:PLayerWithPreview;
begin
  FLayer.Preview:=TPaintBox.Create(pnlLayerPreview);
  FLayer.Preview.Parent:=pnlLayerPreview;
  FLayer.Preview.Align:=alTop;
  FLayer.Preview.Height:=75;
  FLayer.Preview.OnClick:=pbLayerPreviewClick;
  FLayer.Preview.OnPaint:=pbLayerPreviewPaint;
  FLayer.Preview.Cursor:=crHandPoint;
  FLayer.Preview.Tag:=integer(FLayer);

  if LayerList.Count>0 then
  begin
    tmp:=LayerList.Items[LayerList.Count-1];
    FLayer.Preview.Top:=tmp.Preview.Height + tmp.Preview.Top;
  end;

end;

procedure TfrmMain.AddLayerFromStream(AStream:TMemoryStream);
var
  FLayer,tmp:PLayerWithPreview;
begin

  New(FLayer);
  AStream.Read(FLayer.Layer,sizeof(FLayer.Layer));

  if LayerList.Count>0 then
  begin
    tmp:=LayerList.Items[0];
    if (tmp.Layer.LayerType=ltAC) and (FLayer.Layer.LayerType=ltAC) then
    begin
      MessageBox(Handle,'The imported layer is also an AC Layer and your project has already one.',PChar(Application.Title),MB_OK + MB_ICONINFORMATION);
      Dispose(FLayer);
      Exit;
    end;

    tmp:=LayerList.Items[LayerList.Count-1];
    if (tmp.Layer.LayerType=ltSubGrade) and (FLayer.Layer.LayerType=ltSubGrade) then
    begin
      MessageBox(Handle,'The imported layer is also a Subgrade Layer and your project has already one.',PChar(Application.Title),MB_OK + MB_ICONINFORMATION);
      Dispose(FLayer);
      Exit;
    end;
  end;

//  AddToHistory;
  case FLayer.Layer.LayerType of
    ltAC:FLayer.Caption:='ASPHALT CONCRETE';
    ltBase:FLayer.Caption:='BASE LAYER';
    ltSubbase:FLayer.Caption:='SUBBASE LAYER';
    ltSubgrade:FLayer.Caption:='SUBGRADE LAYER';
  end;
  CreateLayerPreviewComponent(FLayer);

  if FLayer.Layer.LayerType=ltAC then
  begin
    LayerList.Insert(0,FLayer);
    FLayer.Preview.Top:=0;
  end
  else
  begin
    LayerList.Add(FLayer);
    if LayerList.Count>=2 then
    begin
      tmp:=LayerList.Items[LayerList.Count-2];
      if tmp.Layer.LayerType=ltSubgrade then
      begin
        LayerList.Exchange(LayerList.Count-2,LayerList.Count-1);
        tmp.Preview.Top:=FLayer.Preview.Top + FLayer.Preview.Height;
      end;
    end;
  end;

  Modified:=true;
end;

procedure TfrmMain.mnuProjectACClick(Sender: TObject);
var
  szFileName:string;
begin
  case (Sender as TdxBarButton).Tag of
    0:szFileName:=ExtractFilePath(ParamStr(0)) + 'DefAC.dlf';
    1:szFileName:=ExtractFilePath(ParamStr(0)) + 'DefBase.dlf';
    2:szFileName:=ExtractFilePath(ParamStr(0)) + 'DefSubbase.dlf';
    3:szFileName:=ExtractFilePath(ParamStr(0)) + 'DefSubgrade.dlf';
  end;
  if FileExists(szFileName) then
    AddLayerFromFile(szFileName)
  else
    MessageBox(Handle,'Default layer information file could not found. Please reinstall program!',PChar(Application.Title),MB_OK + MB_ICONERROR);
end;

function TfrmMain.SaveProjectToFile(szFileName: string): boolean;
var
  AStream:TMemoryStream;
begin
  AStream:=TMemoryStream.Create;
  SaveProjectToStream(AStream);

  AStream.Position:=0;
  AStream.SaveToFile(szFileName);
  AStream.Free;

  Modified:=false;
  Result:=true;
end;

procedure TfrmMain.AddLayerFromFile(szFileName: string);
var
  tmp:TMemoryStream;
  Sign:array [0..4]of char;
  Version:double;
begin

  tmp:=TMemoryStream.Create;
  tmp.LoadFromFile(szFileName);
  tmp.Position:=0;

  tmp.Read(Sign,High(Sign));
  tmp.Read(Version,sizeof(Version));

  if (string(Sign)=LayerSign) and (Version=LayerFileVersion) then
  begin
    AddToHistory;
    AddLayerFromStream(tmp);
  end
  else
    MessageBox(Handle,'Not a Layer information file or incorrect version!',PChar(Application.Title),MB_OK + MB_ICONINFORMATION);
  tmp.Free;
end;

function TfrmMain.OpenProjectFromFile(szFileName: string): boolean;
var
  AStream:TMemoryStream;
begin
  AStream:=TMemoryStream.Create;
  AStream.LoadFromFile(szFileName);
  AStream.Position:=0;

  if OpenProjectFromStream(AStream) then
  begin
    nbNavigator.Visible:=mnuViewNavBar.Down;
    MainSplitter.Visible:=mnuViewNavBar.Down;
    MainSplitter.Left:=nbNavigator.Width;

    AllowPageChange:=true;
    PageControl.ActivePage:=tbOpenProject;
    AllowPageChange:=false;
    PageControl.Visible:=true;

    Modified:=false;
    ProjectFileName:=szFileName;
    Result:=true;
  end
  else
    Result:=false;
  AStream.Free;
  ClearHistory;
end;

procedure TfrmMain.Label31Click(Sender: TObject);
begin
  ShowMEssage('System preview is not ready');
end;

procedure TfrmMain.SetProjectFileName(const Value: string);
begin
  FProjectFileName := Value;
  if Value<>'' then
  begin
    if Modified then
      self.Caption:=Application.Title + ' - *' + Value
    else
      self.Caption:=Application.Title + ' - ' + Value;
  end
  else
    self.Caption:=Application.Title;
end;

procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  //Brush'lar� sil
  BrushBitmaps[0].Free;
  BrushBitmaps[1].Free;
  BrushBitmaps[2].Free;
  BrushBitmaps[3].Free;

  //Listeleri haz�rla
  LayerList.Free;

  WheelBitmap.Free;
end;

procedure TfrmMain.Button1Click(Sender: TObject);
begin
  dxLayerInspector.ShowCustomizeForm;
end;

procedure TfrmMain.Button2Click(Sender: TObject);
var
  AStream:TMemoryStream;
begin
  AStream:=TMemoryStream.Create;
  dxLayerInspector.SaveNodesToStream(AStream);
  AStream.Position:=0;

  AStream.SaveToFile('c:\Output.lip');
  AStream.Free;
end;

procedure TfrmMain.ScaleLayerPreviews;
var
  Ratio:double;
  FLayer:PLayerWithPreview;
  I:integer;
  Total:integer;
begin
  Ratio:=LayerPreviewZoom;
  Total:=0;
  for I:=0 to LayerList.Count-1 do
  begin
    FLayer:=LayerList.Items[I];
    FLayer.Preview.Height:=Round(GetLayerDisplaySize(FLayer,DisplayInfo.PixelPerInch) * Ratio);
    Total:=Total + FLayer.Preview.Height;
  end;
  pnlLayerPreview.Height:=Total;

  {
  if (DisplayInfo.MeshWidth=-1) or (DisplayInfo.MeshWidth=0) then
  begin
    pnlLayerPreview.Width:=Round(Mesh.Width * DisplayInfo.PixelPerInch * Ratio);
    Mesh.IsScaled:=false;
  end
  else
  begin
    Mesh.IsScaled:=true;
    if DisplayInfo.MeshWidth<0 then
      pnlLayerPreview.Width:=Round(Mesh.Width * DisplayInfo.PixelPerInch * Ratio / abs(DisplayInfo.MeshWidth))
    else
      pnlLayerPreview.Width:=Round(DisplayInfo.MeshWidth * DisplayInfo.PixelPerInch * Ratio);
  end;
  }

end;

procedure TfrmMain.CheckLayerHeights;
var
  I:integer;
  FLayer:PLayerWithPreview;
  TotalHeight:double;
begin
  TotalHeight:=0;
  for I:=0 to LayerList.Count-2 do
  begin
    FLayer:=LayerList.Items[I];
    TotalHeight:=TotalHeight + FLayer.Layer.Thickness;
  end;

  if LayerList.Count=0 then Exit;

  FLayer:=LayerList.Last;
  if FLayer.Layer.LayerType=ltSubgrade then
  begin
    FLayer.Layer.Thickness:=EnsureRange(Mesh.Depth - TotalHeight,0,Mesh.Depth);
    if FLayer.Layer.Thickness=0 then
      MessageBox(Handle,'The Depth of Sugrade layer was truncated to 0. Depth of the system is larger than Mesh System. Please check your layers!',PChar(Application.Title),MB_OK + MB_ICONWARNING);
    TotalHeight:=Mesh.Depth;
  end
  else
  begin
    TotalHeight:=TotalHeight + FLayer.Layer.Thickness;
    if TotalHeight > Mesh.Depth then
      MessageBox(Handle,'Depth of the system is larger than Mesh System. Please check your layers!',PChar(Application.Title),MB_OK + MB_ICONWARNING);
  end;

  pnlLayerPreview.Height:=Round(TotalHeight * LayerPreviewZoom);
end;

procedure TfrmMain.SetLayerPreviewZoom(const Value: double);
begin
  FLayerPreviewZoom := Value;
  ScaleLayerPreviews;
  pbVerticalRuler.Repaint;
  pbHorizontalRuler.Repaint;
  pbWheel.Repaint;

  PostMessage(Handle,WM_REFRESHZOOM,0,0);
end;

procedure TfrmMain.pnlLayerPreviewResize(Sender: TObject);
begin
  FResizing:=true;
  ScaleLayerPreviews;
  pbHorizontalRuler.Width:=pbHorizontalRuler.Parent.Width - pbHorizontalRuler.Left - sbPreviewScroll.Width; 
  pnlLayerPreview.Width:=pnlLayerPreview.Parent.Parent.ClientWidth - sbPreviewScroll.Width - 2 - pbVerticalRuler.Width;
  if pnlLayerPreview.Height>pnlLayerPreview.Parent.Parent.ClientHeight - pbHorizontalRuler.Height - pbWheel.Height then
  begin
    sbPreviewScroll.Position:=EnsureRange(sbPreviewScroll.Position,sbPreviewScroll.Min,pnlLayerPreview.Height - pnlLayerPreview.Parent.Parent.ClientHeight + pbHorizontalRuler.Height + pbWheel.Height);
    sbPreviewScroll.Max:=pnlLayerPreview.Height - pnlLayerPreview.Parent.Parent.ClientHeight + pbHorizontalRuler.Height + pbWheel.Height;
    sbPreviewScroll.Enabled:=true;
    pbVerticalRuler.Height:=pnlLayerPreview.Height;
  end
  else
  begin
    sbPreviewScroll.Enabled:=false;
    sbPreviewScroll.Position:=0;
    pnlLayerPreview.Top:=0;
    pbVerticalRuler.Top:=pbHorizontalRuler.Height + pbWheel.Height;
    pbVerticalRuler.Height:=pbVerticalRuler.Parent.Height;
  end;

  pnlLayerPreview.Parent.ClientHeight:=pnlLayerPreview.Height;
  pnlLayerPreview.Parent.ClientWidth:=pnlLayerPreview.Width;

  FResizing:=false;
end;

procedure TfrmMain.sbPreviewScrollScroll(Sender: TObject;
  ScrollCode: TScrollCode; var ScrollPos: Integer);
begin
  if FResizing then Exit;

  pnlLayerPreview.Top:=-sbPreviewScroll.Position;
  pbVerticalRuler.Top:=-sbPreviewScroll.Position + pbHorizontalRuler.Height + pbWheel.Height;
end;

procedure TfrmMain.mnuZoomInClick(Sender: TObject);
begin
  LayerPreviewZoom:=LayerPreviewZoom * 2;
end;

procedure TfrmMain.mnuZoomOutClick(Sender: TObject);
begin
  LayerPreviewZoom:=LayerPreviewZoom / 2;
end;

function TfrmMain.SaveProjectToStream(AStream: TMemoryStream): boolean;
var
  I:integer;
  FLayer:PLayerWithPreview;
  Sign:array[0..4]of char;
  Version:double;
begin
  StrPCopy(Sign,ProjectSign);
  Version:=ProjectFileVersion;

  AStream.Write(Sign,High(Sign));
  AStream.Write(Version,sizeof(Version));

  AStream.Write(ProjectInfo,sizeof(ProjectInfo));
  AStream.Write(WheelInfo,sizeof(WheelInfo));

  I:=LayerList.Count;
  AStream.Write(I,sizeof(I));
  for I:=0 to LayerList.Count-1 do
  begin
    FLayer:=LayerList.Items[I];
    AStream.Write(FLayer.Layer,sizeof(FLayer.Layer));
  end;

  Result:=true;
end;

procedure TfrmMain.AddToHistory;
var
  tmp:TMemoryStream;
begin
  if RestoreOnProgress then Exit;

  tmp:=TMemoryStream.Create;
  SaveProjectToStream(tmp);
  tmp.Position:=0;
  UndoList.Add(tmp);
  mnuProjectUndo.Enabled:=true;
end;

procedure TfrmMain.StepBackFromHistory;
var
  tmp:TMemoryStream;
  szProjectNameBackup:string;
  ActiveLayerBackup:PLayerWithPreview;
begin
  if UndoList.Count>0 then
  begin
    RestoreOnProgress:=true;

    szProjectNameBackup:=ProjectFileName;
    ActiveLayerBackup:=ActiveLayer;

    tmp:=UndoList.Last;

    Modified:=false;
    CloseProject(false);
    OpenProjectFromStream(tmp);
    tmp.Free;
    UndoList.Delete(UndoList.Count-1);

    cmdRestoreWheelClick(cmdRestoreWheel);
    cmdRestoreExecutionClick(cmdRestoreExecution);

    ProjectFileName:=szProjectNameBackup;
    Modified:=true;

    if LayerList.IndexOf(ActiveLayerBackup)>=0 then
      ActiveLayerBackup.Preview.OnClick(ActiveLayerBackup.Preview);

    RestoreOnProgress:=false;
  end;
  if UndoList.Count=0 then
    mnuProjectUndo.Enabled:=false
  else
    mnuProjectUndo.Enabled:=true;
end;

function TfrmMain.OpenProjectFromStream(AStream: TMemoryStream): boolean;
var
  LayerCount:integer;
  I:integer;
  Sign:array[0..4]of char;
  Version:double;
begin
  AStream.Read(Sign,High(Sign));
  AStream.Read(Version,sizeof(Version));
  if (string(Sign)=ProjectSign) and (Version=ProjectFileVersion) then
  begin
    AStream.Read(ProjectInfo,sizeof(ProjectInfo));
    AStream.Read(WheelInfo,sizeof(WheelInfo));

    AStream.Read(LayerCount,sizeof(LayerCount));
    for I:=0 to LayerCount-1 do
      AddLayerFromStream(AStream);

    CheckLayerHeights;
    ScaleLayerPreviews;

    Result:=true;
  end
  else
  begin
    MessageBox(Handle,'Not a valid project file or incorrect version!',PChar(Application.Title),MB_OK + MB_ICONINFORMATION);
    Result:=false;
  end;
end;

procedure TfrmMain.ClearHistory;
var
  I:integer;
  tmp:TMemoryStream;
begin
  for I:=0 to UndoList.Count-1 do
  begin
    tmp:=UndoList.Items[I];
    tmp.Free;
  end;
  UndoList.Clear;

  mnuProjectUndo.Enabled:=false;
end;

procedure TfrmMain.mnuProjectUndoClick(Sender: TObject);
begin
  StepBackFromHistory;
end;

function TfrmMain.GetLayerDisplaySize(FLayer: PLayerWithPreview;DPI:integer): integer;
var
  tmp:integer;
begin
  tmp:=0;
  case FLayer.Layer.LayerType of
    ltAC:tmp:=DisplayInfo.ACSize;
    ltBase:tmp:=DisplayInfo.BaseSize;
    ltSubbase:tmp:=DisplayInfo.SubbaseSize;
    ltSubgrade:tmp:=DisplayInfo.SubgradeSize;
  end;

  if (tmp=0) or (tmp=-1) then
  begin
    Result:=Round(FLayer.Layer.Thickness * DPI);
    FLayer.IsScaled:=false;
    Exit;
  end;

  FLayer.IsScaled:=true;

  if tmp<0 then
  begin
    Result:=Round(FLayer.Layer.Thickness / abs(tmp) * DPI);
    Exit;
  end;

  Result:=EnsureRange(Round(FLayer.Layer.Thickness),0,tmp);
  Result:=Result * DPI;
end;

procedure TfrmMain.DrawArrow(ACanvas: TCanvas; X, Y, Scale: integer);
begin
  ACanvas.PenPos:=Point(X,Y);
  ACanvas.Polygon([Point(X,Y), Point(X + Scale, Y),Point(X+Scale,Y + Scale * 3), Point(X+Scale*2, Y + Scale * 3),
                   Point(X + Scale div 2, Y + Scale * 4),Point(X - Scale, Y + Scale * 3),Point(X, Y + Scale * 3)]);
end;

procedure TfrmMain.pbHorizontalRulerPaint(Sender: TObject);
var
  I:integer;
  Step:integer;
  Counter:integer;
begin

  pbHorizontalRuler.Canvas.Brush.Color:=clWhite;
  pbHorizontalRuler.Canvas.FillRect(pbHorizontalRuler.ClientRect);

  I:=pbVerticalRuler.Width-1;

  //Step, Layer Height gibi hesaplanacak
  if (DisplayInfo.MeshWidth=0) or (DisplayInfo.MeshWidth=-1) then
    Step:=Round(DisplayInfo.PixelPerInch * LayerPreviewZoom)
  else
  begin
    if DisplayInfo.MeshWidth<0 then
      Step:=Round(Mesh.Width / abs(DisplayInfo.MeshWidth * DisplayInfo.PixelPerInch ) * LayerPreviewZoom)
    else
      Step:=(pbHorizontalRuler.Width - I) div DisplayInfo.MeshWidth;
  end;
  Counter:=0;

  pbHorizontalRuler.Canvas.PenPos:=Point(I,pbHorizontalRuler.Height - 1);
  pbHorizontalRuler.Canvas.LineTo(pbHorizontalRuler.Width,pbHorizontalRuler.Height - 1);
  while (I<pbHorizontalRuler.Width) do
  begin

    pbHorizontalRuler.Canvas.PenPos:=Point(I,pbHorizontalRuler.Height - 5);
    pbHorizontalRuler.Canvas.LineTo(I,pbHorizontalRuler.Height);

//    if Counter>0 then
      pbHorizontalRuler.Canvas.TextOut( I - pbHorizontalRuler.Canvas.TextWidth(IntToStr(Counter)) div 2  ,0,IntToStr(Counter));
    Inc(Counter);
    Inc(I,Step);
  end;
end;

procedure TfrmMain.sbPreviewScrollChange(Sender: TObject);
begin
  if FResizing then Exit;
  pnlLayerPreview.Top:=-sbPreviewScroll.Position;
  pbVerticalRuler.Top:=-sbPreviewScroll.Position + pbHorizontalRuler.Height + pbWheel.Height;
end;

procedure TfrmMain.pbVerticalRulerPaint(Sender: TObject);
var
  I:integer;
  Step:integer;
  Counter:integer;
begin
  I:=0;

  pbVerticalRuler.Canvas.Brush.Color:=clWhite;
  pbVerticalRuler.Canvas.FillRect(pbVerticalRuler.ClientRect);

  //Step, Layer Height gibi hesaplanacak
  Step:=Round(DisplayInfo.PixelPerInch * LayerPreviewZoom);
  Counter:=0;
  pbVerticalRuler.Canvas.PenPos:=Point(pbVerticalRuler.Width-1,0);
  pbVerticalRuler.Canvas.LineTo(pbVerticalRuler.Width-1,pbVerticalRuler.Height);

  while (I<pbVerticalRuler.Height) do
  begin
    pbVerticalRuler.Canvas.PenPos:=Point(pbVerticalRuler.Width-5,I);
    pbVerticalRuler.Canvas.LineTo(pbVerticalRuler.Width,I);

    if Counter mod 2 = 1 then
      pbVerticalRuler.Canvas.TextOut( 0 ,I - pbVerticalRuler.Canvas.TextHeight(IntToStr(Counter)) div 2 ,IntToStr(Counter));
    Inc(Counter);
    Inc(I,Step);
  end;
end;

procedure TfrmMain.pbWheelPaint(Sender: TObject);
var
  ARect:TRect;
  Step:integer;
  I:integer;
  VirtualWidth:integer;
  ArrowPos:integer;
begin

  pbWheel.Canvas.Brush.Color:=clWhite;
  ARect:=Rect(0,0,pbVerticalRuler.Width,pbWheel.Height);
  pbWheel.Canvas.FillRect(ARect);

  pbWheel.Canvas.Pen.Color:=clBlack;
  pbWheel.Canvas.PenPos:=Point(pbVerticalRuler.Width-1,0);
  pbWheel.Canvas.LineTo(pbVerticalRuler.Width-1,pbWheel.Height);


  //Step, Layer Height gibi hesaplanacak
  I:=pbVerticalRuler.Width-1;
  if (DisplayInfo.MeshWidth=0) or (DisplayInfo.MeshWidth=-1) then
    Step:=Round(DisplayInfo.PixelPerInch * LayerPreviewZoom)
  else
  begin
    if DisplayInfo.MeshWidth<0 then
      Step:=Round(Mesh.Width / abs(DisplayInfo.MeshWidth * DisplayInfo.PixelPerInch ) * LayerPreviewZoom)
    else
      Step:=(pbHorizontalRuler.Width - I) div DisplayInfo.MeshWidth;
  end;

  VirtualWidth:=Round(WheelInfo.Radius * Step);


  //Teker resimlerini at
  DrawWheel(pbWheel.Canvas,pbVerticalRuler.Width + 10,5);
  DrawWheel(pbWheel.Canvas,pbVerticalRuler.Width,8);

  DrawWheel(pbWheel.Canvas,VirtualWidth + 5,5);
  DrawWheel(pbWheel.Canvas,VirtualWidth -5 ,8);

  //Ba�a Arrow at
  pbWheel.Canvas.Pen.Color:=clGray;
  pbWheel.Canvas.Brush.Color:=clBlack;
  DrawArrow(pbWheel.Canvas,pbVerticalRuler.Width,pbWheel.Height - 26,5);
  pbWheel.Canvas.Pen.Color:=clRed;
  pbWheel.Canvas.Brush.Color:=clMaroon;
  DrawArrow(pbWheel.Canvas,pbVerticalRuler.Width,pbWheel.Height - 25,5);

  //Sona Arrow at
  pbWheel.Canvas.Pen.Color:=clGray;
  pbWheel.Canvas.Brush.Color:=clBlack;
  DrawArrow(pbWheel.Canvas,VirtualWidth + pbVerticalRuler.Width - 3,pbWheel.Height - 26,5);
  pbWheel.Canvas.Pen.Color:=clRed;
  pbWheel.Canvas.Brush.Color:=clMaroon;
  DrawArrow(pbWheel.Canvas,VirtualWidth + pbVerticalRuler.Width - 3,pbWheel.Height - 25,5);


  ArrowPos:=VirtualWidth div 4 + pbVerticalRuler.Width;
  while (ArrowPos < VirtualWidth + pbVerticalRuler.Width - 4) do
  begin
    pbWheel.Canvas.Pen.Color:=clGray;
    pbWheel.Canvas.Brush.Color:=clBlack;
    DrawArrow(pbWheel.Canvas,ArrowPos + 2,pbWheel.Height - 26,5);

    pbWheel.Canvas.Pen.Color:=clRed;
    pbWheel.Canvas.Brush.Color:=clMaroon;
    DrawArrow(pbWheel.Canvas,ArrowPos ,pbWheel.Height - 25,5);

    ArrowPos:=ArrowPos + VirtualWidth div 4;
  end;



end;

procedure TfrmMain.DrawWheel(ACanvas: TCanvas; X, Y:integer);
begin
  WheelBitmap.Transparent:=true;
  ACanvas.Draw(X,Y,WheelBitmap);
end;

procedure TfrmMain.WmRefreshZoom(var Msg: TMessage);
begin
  mnuViewZoom.Caption:='Ratio:' + FloatToStrF(self.LayerPreviewZoom * 100,ffFixed,15,2) + ' %';
end;

end.
