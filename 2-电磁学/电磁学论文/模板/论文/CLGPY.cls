%-------------------------------------CLGPY--------------------------------------------%
% 作者：待定
% 邮箱：dyu@mail.ynu.edu.cn
% 时间: 2020-1-5
%--------------------------------几点说明----------------------------------------------%
% 按《大学物理》期刊官方论文模板定制的非官方LaTeX模板，请酌情使用！
%
% 若要投稿，还是请用官方提供的word模板，以免后续徒增麻烦。
%
% 首次编写文类，难免存在一些错误，在使用时如发现错误或对此模板有更好的建议，还望不吝赐教。
%
%--------------------------------------------------------------------------------------%

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{CLGPY}
\LoadClass[zihao=5]{ctexart}

%=============================三级标题(节、小节、小小节)格式设置=======================%
\ctexset{
    section/format         +=\raggedright\zihao{-4},
    section/aftername       =\kern\ccwd,
    subsection/format      +=\raggedright\zihao{5},
    subsection/aftername    =\kern\ccwd,
    subsubsection/format   +=\raggedright\zihao{-5},
    subsubsection/aftername =\kern\ccwd
}

%=========================================英文年月简写================================%
\def\enianyue{
    \ifcase\the\month
    \or Jan%
    \or Feb%
    \or Mar%
    \or Apr%
    \or May%
    \or Jun%
    \or Jul%
    \or Aug%
    \or Sep%
    \or Oct%
    \or Nov%
    \or Dec%
    \else No idea!%
    \fi.~\the\year
}

%%======================================中文题名信息设置===============================%
\renewcommand{\title}[2][\@title]{\gdef\@tle{#1}\gdef\@title{#2}}%重定义标题
\def\@title{\@latex@error{No \noexpand\title given}\@ehc}%如果没有提供标题命令，系统将会停止编译并给出错误信息
\def\@tle{\@latex@warning{Without \noexpand\title being given, naturally there is no its <options>}}%如果没有提供标题命令，系统将会附带给出一个关于其可选参数的警告信息
\renewcommand{\author}[2][\@author]{\gdef\@atr{#1}\gdef\@author{#2}}%重定义作者
\def\@author{\@latex@error{No \noexpand\author given}\@ehc}%如果没有提供作者命令，系统将会停止编译并给出错误信息
\def\@atr{\@latex@warning{Without \noexpand\author being given, naturally there is no its <options>}}%如果没有提供作者命令，系统将会附带给出一个关于其可选参数的警告信息
\def\institute#1{\gdef\@institute{#1}}%定义机构
\def\@institute{\@latex@warning{No \noexpand\institute given}}%如果没有提供机构命令，系统将会给出警告信息
\def\juanqi#1#2{\gdef\@juan{#1}\gdef\@qi{#2}}%定义卷期
\def\@juan{\@latex@error{No \noexpand\juanqi given}\@ehc}%如果没有提供卷期命令，系统将会停止编译并给出错误信息
\def\@qi{\@latex@error{No \noexpand\juanqi given}\@ehc}%如果没有提供卷期命令，系统将会停止编译并给出错误信息
\def\@maketitle{%
    \null
    \vskip.5em%
    \begin{center}%
        {\zihao{-2}\heiti \@title \par}%标题小二黑体
        \vskip.5em%
        {\zihao{-4}\fangsong%作者小四仿宋
        \lineskip=.5em%
        \begin{tabular}[t]{c}%
            \@author
        \end{tabular}\par}%
        \vskip.3em%
        {\zihao{6}\songti\@institute}%机构六号宋体
    \end{center}%
    \par
    \vskip1em%
    }
\def\syym{%定义首页页眉
     \vspace*{-13mm}%
     \noindent \hbox to \textwidth{\makebox[.33\textwidth][l]{\small  \@juan   \@qi  }\makebox[.33\textwidth][c]{\small 电磁学C：课程论文}\makebox[.33\textwidth][r]{\small \@juan \kern5pt  \@qi}}%
      \par\kern-7pt\noindent \hbox to \textwidth{\makebox[.33\textwidth][l]{\small \the\year 年 \the\month 月}\makebox[.33\textwidth][c]{\small Electromagnetics \enspace Essay}\makebox[.33\textwidth][r]{\small\enianyue}}%
        \par\kern-10pt\noindent {\rule[.5mm]{\textwidth}{.5pt}\hspace*{-\textwidth}\rule{\textwidth}{.5pt}}%
    }
\renewcommand\maketitle{\par
   \begingroup
     \thispagestyle{empty}%将标题页所在页眉清空
     \let\footnote\thanks%将脚注命令等效于\thanks致谢命令
     \renewcommand{\thefootnote}{\@fnsymbol\c@footnote}%\@fnsymbol数字：重定义脚注序号命令(*、\dagger等标识符计数形式)，通常数字是用\c@footnote等计数器表示
     \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%\rlap将对象装入宽度为0的盒子并置于插入点的右侧；\@textsuperscript用于生成上标；\@thefnmark用于获取脚注序号。这里是使脚注序号不对原信息造成影响
     \long\def\@makefntext##1{\hspace*{\ccwd}\noindent
               \hbox to 1.8em{\@textsuperscript{\normalfont\@thefnmark}##1}
        }%定义\@makefntext##1脚注文本命令，将脚注标识符装进1.8em宽的水平盒子，以免脚注标识符与其后的文本重叠，\onindent使脚注文本紧跟在脚注标识符之后而不另起一行，参数##1即为脚注文本
     \syym%画出标题页的页眉
     \ifnum \col@number=\@ne%\col@number用于指示当前栏数
         \@maketitle%若为单栏则直接排版题名信息
     \else
         \twocolumn[\@maketitle]%若为双栏仍按单栏排版题名信息，其后文本改用双栏排版
     \fi
     \@thanks
   \endgroup
   \setcounter{footnote}{0}%在题名页结束时，将脚注序号计数器清零
   \global\let\thanks\relax
   \global\let\maketitle\relax
   \global\let\@maketitle\relax
   \global\let\@thanks\@empty
   \global\let\@institute\@empty
   \global\let\title\relax
   \global\let\author\relax
   \global\let\institute\relax
   \global\let\syym\relax
   \global\let\juanqi\relax
   \global\let\and\relax
}%题名完成后，将系统内部和外部不再用到的的题名信息命令废止或清空，也就是说这些命令只能使用一次

%======================================英文题名信息设置===============================%
\def\Title#1{\gdef\@Title{#1}}
\def\@Title{\@latex@warning{No \noexpand\Title given}}%如果没有提供标题命令，系统将会给出警告信息
\def\Author#1{\gdef\@Author{#1}}
\def\@Author{\@latex@warning{No \noexpand\Author given}}%如果没有提供作者命令，系统将会给出警告信息
\def\Institute#1{\gdef\@Institute{#1}}
\def\@Institute{\@latex@warning{No \noexpand\Institute given}}%如果没有提供机构命令，系统将会给出警告信息
\def\@Maketitle{%
    \null
    \vskip.3em%
    \begin{center}%
        {\zihao{-3}\textbf{\@Title} \par}%标题小三加黑
        \vskip.5em%
        {\zihao{-4}%作者小四
        \lineskip=.5em%
        \begin{tabular}[t]{c}%
            \@Author
        \end{tabular}\par}%
        \vskip.3em%
        {\zihao{-5}\@Institute}%机构小五
    \end{center}%
    \par
    \vskip1em%
}  
\def\Maketitle{\par%
   \begingroup
    \ifnum \col@number=\@ne
        \@Maketitle
    \else
        \twocolumn[\@Maketitle]
    \fi
   \endgroup
   \global\let\@maketitle\relax
   \global\let\@Title\@empty
   \global\let\@Author\@empty
   \global\let\@Institute\@empty
   \global\let\maketitle\relax
   \global\let\title\relax
   \global\let\author\relax
   \global\let\institute\relax
}%题名完成后，将系统内部和外部不再用到的的题名信息命令废止或清空，也就是说这些命令只能使用一次

%=====================================必要脚注信息设置===============================%
\renewcommand{\thefootnote}{}%清除脚注序号
\long\def\@makefntext#1{\hspace*{\ccwd}\noindent #1}
\def\sxrq#1#2{\footnotetext{\zihao{6}{\heiti 收稿日期：\thinspace}#1；\enskip{\heiti 修改日期：\thinspace}#2}}
\def\jjxm#1{\footnotetext{\zihao{6}{\heiti 基金项目：\thinspace}#1}}
\def\zzjj#1{\footnotetext{\zihao{6}{\heiti 作者简介：\thinspace}#1}}
\def\txzz#1{\footnotetext{\zihao{6}{\heiti 通信作者：\thinspace}#1}}

%===================================中英文摘要、关键词定义==========================%
\def\keywords#1{\parindent=2\ccwd {\zihao{-5}\textbf{关键词：}#1}}
\def\enkeywords#1{\parindent=2\ccwd {\zihao{5}\textbf{Key words：}#1}}
%-----------------------------------------------------------------------------------%
\renewcommand{\abstractname}{\zihao{-5} \textbf{摘要：}}
\renewenvironment{abstract}[1]{\par\zihao{-5}\parindent 2\ccwd \abstractname #1}{\par} 
\newcommand{\enabstractname}{\zihao{5}\textbf{Abstract:}}
\newenvironment{enabstract}[1]{\par\zihao{5}\parindent 2\ccwd \enabstractname #1}{\par}

%======================================必要信息定义及设置============================%
\def\xinxi#1#2#3#4{{\zihao{-5}\heiti%
    \par\parindent=2\ccwd \mbox{中图分类号：#1} \kern3\ccwd \mbox{文献标识码：#2} \kern3\ccwd \mbox{文章编号：#3}%
    \par\parindent=2\ccwd \mbox{【DOI】#4}}}

%=========================================版式定义与设置=============================%
\@twosidetrue
\if@twoside
\def\ps@clgpy{%定义一个新的版式clgpy
    \let\@oddfoot\empty%清空奇数页页脚
    \let\@evenfoot\empty%清空偶数页页脚
    \def\@oddhead{\small \makebox[.33\textwidth][l]{ \@qi  }\hfil \makebox[.33\textwidth][c]{\@atr ：\@tle} \hfil \makebox[.33\textwidth][r]{\thepage}\hspace*{-\textwidth}\rule[-2mm]{\textwidth}{.5pt}}%定义奇数页页眉
    \def\@evenhead{\small \makebox[.33\textwidth][l]{\thepage}\hfil \makebox[.33\textwidth][c]{电磁学C：课程论文}\hfil \makebox[.33\textwidth][r]{ \@juan  }\hspace*{-\textwidth}\rule[-2mm]{\textwidth}{.5pt}}%定义偶数页页眉
    }
\fi
\pagestyle{clgpy}%使用新定义的版式

%=======================================设置栏间距====================================%
\setlength\columnsep{20\p@}

%======================================一些有用的宏包=================================%
%--------------------字体设置宏包-----------------%
\RequirePackage{fontspec}
\setmainfont{Times New Roman}%全文数字、西文用Times New Roman

%----------------------分栏宏包-------------------%
\RequirePackage{multicol}
%实用命令：
%\begin{multicols}{n}
% 	分成n栏的内容
%\end{multicols}

%-------------------图表标题控制宏包--------------%
\RequirePackage{caption}
\DeclareCaptionFont{zihao}{\zihao{#1}}%自定义标题字体格式
\DeclareCaptionLabelSeparator{ccwd}{\hspace*{\ccwd}}%自定义标题标签分隔符为单个汉字字宽
\DeclareCaptionLabelSeparator{cwd}{\hspace*{.5\ccwd}}%自定义标题标签分隔符为半个汉字字宽
\captionsetup[figure]{
    font={zihao=-5},
    labelsep=cwd,
    justification=centering
}
\captionsetup[table]{
    font={zihao=-5,bf},
    labelsep=ccwd,
    skip=0pt,
    justification=centering
}

\RequirePackage{subcaption}
\captionsetup[sub]{font={zihao=-5}}
%-------------------文献格式定制宏包--------------%
\RequirePackage[numbers,sort&compress]{natbib}
\setcitestyle{super}
\renewcommand\bibfont{\zihao{-5}}

%--------------------浮动控制宏包-----------------%
\RequirePackage{float}%为标准的浮动环境提供了一个新的H位置选项来产生没有浮动效果的图表环境

%-------------------表格宏包----------------%
\RequirePackage{booktabs}%三线表绘制
\RequirePackage{array}%列格式控制

%----------------------数学宏包-------------------%
\RequirePackage{amsmath}%AMS数学公式扩展，已成为LaTeX必备宏包。
% 1.\text{}在数学公式中插入文字。
% 2.\sideset{左_^}{右^_}元素,此命令仅用于排版巨算符的角标，不应用在其他地方。
% 3.\overset{上标记}{符号} \underset{下标记}{符号}
% 4.\genfrac{左括号}{右括号}{线宽(若为0pt，则没有分数线；若为空则表示默认)}{大小(由大到小0,1,2,3)，若为空则表示默认}{分子}{分母}
% 5.\uproot{整数(无单位)，可正可负}，\leftroot{整数(无单位)，可正可负}对开放次数的排版进行调整，移动的单位是很小的一段距离。
% 6.\hdotsfor{列数}跨多列的省略号。
% 7.smallmatrix环境，排版行内公式矩阵。
% 8.\substack{条件\\条件\\……}排版巨算符上下限多行条件时用，效果相当于只有一列的无括号的矩阵；类似地，subarray环境也可用在这种环境，但可以指定对齐方式(l,c,r),如\begin{subarray}	{对齐方式(由于是一列，所以只能选一个参数)}\end{subarray}
% 9.\pmb伪粗体。
% 10.\stackrel{上}{下}关系符上堆叠符号的效果。
\RequirePackage{mathtools}%数学公式扩展宏包，对amsmath做了进一步扩展提供了公式编号定制和更多的符号、矩阵等。
% 1.\prescript{上标}{下标}{元素}让元素产生间距合理的左上下标
% 2.\overbracket[线宽][伸出高度]{内容} \overbracket[线宽][伸出高度]{内容} 在数学公式上下加方括号。
% 3.\idots反向斜省略号。
% 4.提供了带星号的matrix*系列矩阵环境，可指定可选的列对齐方式，用[一个可选]来装参数。
% 5.\shortintertext可代替\intertext得到更为紧凑的行间距。
% 6.dcases环境，分块时，保证每行公式都是显示格式(displaystyle)大小。
\RequirePackage{xfrac}%\sfrac{分子}{分母}排版分子和分母都很小都很短的小分式，错位斜分式，易于区分。
\RequirePackage{amsxtra}%以\sp开头的命令，上标形式的重音。
\RequirePackage{extarrows}%将可延长箭头(箭头上下方加内容)扩充到其他符号。	
\RequirePackage{cases}%numcases环境使得大括号后每一行公式都有编号,其本身就是数学环境
%\begin{numcases}{左边的子公式}
%<子公式1>&<文本模式下：条件1>\\
%<子公式1>&<文本模式下：条件1>\\
%......
%\end{numcases}
\RequirePackage{siunitx}%以国际单位规范排版物理量的单位。
\RequirePackage{mhchem} 	%排版化学式和方程式。
%\RequirePackage{amsfonts}%AMS扩展符号的基础字体支持。
\RequirePackage{amssymb}%在amsfonts基础上将AMS扩展符号定义成命令,最常用的额外数学字体包。
% 1.\mathbb{}黑板粗体，仅大写字母。
% 2.\mathfrak{}哥特体。
\RequirePackage{mathrsfs}
% 1.\mathscr{}花体，仅大写字母。
\RequirePackage{bm}%提供将数学符号加粗的命令\bm选择粗体数学符号,\pm选择更粗的加重体数学符号。
%\RequirePackage{breqn}小心使用此宏包，尽量放在导言区调用宏包的后面%dmath和dmath*环境，产生可以自动折行的显示公式。
\RequirePackage{mathabx}%产生二重闭积分符号\oiint
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%部分有用命令说明%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1.\mathbin{}将参数看作是二元运算符
% 2.\mathrel{}将参数看作是二元关系符
% 3.\mathpunct{}把一个符号看作数学标点
% 4.\intertext{}在几行公式中插入简短文字，同时又不破坏公示的对其，由于其本身就表示换行，因而前面一行的\\可省去。
% 5.subequations环境，行内多个公式看作一个公式，使用同一个主编号，里面分别按字母对子公式进行编号
% 6.\split环境，其环境开始并不开始一个数学公式也就不会产生编号，要套在数学环境里使用，数学环境来产生编号；可以把单个公式拆分成多行，但不能对其多列公式，用法同align.
% 7.\mspace{...mu}调整数学间距。
% 8.长度变量：(用\setlength改变)
%\abovedisplayskip \abovedisplayshortplan(文字与公式有重叠)调整上文与公式的距离
%\belowdisplayskip \abovedisplayshortskip(文字与公式有重叠)调整下文与公式的距离
%\jot多行公式之间的间距
%\mathsurround控制行内公式与两边文字的额外间距。
% 9.\skew{重音符号左右移动的距离 mu}{数学重音}{被标记的数学符号}调整数学重音的位置
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand\addtag{\refstepcounter{equation}\tag{\theequation}}%定义一个给公式添加编号的命令
\newcommand\du{^\circ}%度数定义

%------------------------页面尺寸设置宏包----------------------%
\RequirePackage[a4paper,hmargin=.8in,vmargin=1in]{geometry}%满足每栏22个中文字符，每页44行的奇怪要求

%----------------------------插图宏包--------------------------%
\RequirePackage{graphicx}

%----------------------------颜色宏包--------------------------%
\RequirePackage{xcolor}

%----------------------------绘图宏包--------------------------%
\RequirePackage{tikz}
\RequirePackage{tikz-3dplot}
\RequirePackage{animate}

%---------------------------超链接宏包--------------------------%
\RequirePackage{hyperref}
\hypersetup{%
	breaklinks=true,
	bookmarks=true,%生成PDF书签
	bookmarksopen=false，%在PDF阅读器中不自动打开书签
	pdfborder=000
}
%自动写入pdf信息
\AtBeginDocument{
    \hypersetup{
        pdftitle   = {\@title},
        pdfauthor  = {\@atr}
    }
}
\renewcommand\figureautorefname{图}
\renewcommand\tableautorefname{表}

%\RequirePackage{nameref}%引用章节或图表标题,hyperref宏包中已有此功能