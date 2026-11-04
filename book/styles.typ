// 版式只在这个文件里维护；内容是 print.py 生成的 build/book.typ，不手改。
// 版式版本：v2（2026-11-01）字段卡片 + 代码卡片。
//
// 设计目标：A4 单面、左边装订、黑白优先；说明做成"字段卡片"，
// 代码做成"代码卡片"，长板允许分页，页眉保留当前章名。
// 改版式请单独提交，并在提交信息里写"版式"，然后重跑
// `python3 print.py --check` 和 `python3 print.py`。
#import "@preview/codly:1.3.0": *

// ---------- 设计变量（改版式先改这里） ----------

#let ink = rgb("#14181B")
#let text-ink = rgb("#2A3136")
#let accent = rgb("#1D4E5F")
#let accent-soft = rgb("#E9F0F2")
#let muted = rgb("#5D686E")
#let faint = rgb("#8B9499")
#let hint = rgb("#78848A")
#let rule = rgb("#D9DEE1")
#let code-fill = rgb("#F7F8F8")
#let code-rule = rgb("#E4E8EA")

#let sans-font = ("Noto Sans CJK SC", "Noto Sans", "DejaVu Sans")
#let mono-font = ("Noto Sans Mono", "Noto Sans Mono CJK SC", "DejaVu Sans Mono")

#let body-size = 9.7pt
#let field-size = 9.2pt
#let code-size = 7.8pt

// ---------- 小部件 ----------

// 复杂度随标题走：生成器在每节前面放一个 metadata，标题样式按位置取。
#let cmplx(value) = metadata((kind: "cmplx", value: value))

// 字段卡片：下标 / 用途 / 核心 / 用法 / 坑 / 例 / 依赖
#let fields(rows) = block(above: 1pt, below: 10pt, width: 100%)[
  #grid(
    columns: (4.3em, 1fr),
    column-gutter: 0.6em,
    row-gutter: 0.42em,
    ..rows.map(((label, body)) => (
      if label == "" {
        []
      } else {
        text(
          font: sans-font,
          size: 8.1pt,
          weight: "bold",
          fill: accent,
          tracking: 0.02em,
        )[#label]
      },
      text(font: sans-font, size: field-size, fill: text-ink, body),
    )).flatten(),
  )
]

// 行内示例：等宽 + 左侧细线，和正文区分开。
#let example(body) = block(
  width: 100%,
  inset: (left: 7pt, top: 1.2pt, bottom: 1.2pt),
  stroke: (left: 1.6pt + rule),
  text(font: mono-font, size: 8.3pt, fill: text-ink, body),
)

// 代码卡片：外框和留白在这里，codly 只管行号和语法色。
#let code(body) = block(
  width: 100%,
  fill: code-fill,
  stroke: 0.3pt + code-rule,
  radius: 2pt,
  inset: (x: 9pt, top: 5pt, bottom: 5pt),
  body,
)

#let book(
  title: "",
  name: "",
  short: "",
  subtitle: "",
  edition: "",
  author: "",
  count: 0,
  chapters: (),
  duplex: false,
  body,
) = {
  show: codly-init.with()

  set document(title: title, author: author)
  set text(font: sans-font, size: body-size, fill: text-ink, lang: "zh", region: "cn")
  set par(justify: false, leading: 0.55em, spacing: 0.65em)
  set heading(numbering: "1.1")
  set raw(theme: "/book/theme.tmTheme", tab-size: 4)

  show raw: it => {
    set text(font: mono-font, size: code-size)
    // raw 的每一行也受 par 影响；不收紧的话代码会变成两倍行距。
    set par(leading: 0.28em, spacing: 0.28em)
    it
  }
  codly(
    languages: (cpp: (name: "C++", icon: none)),
    display-name: false,
    display-icon: false,
    zebra-fill: none,
    fill: none,
    stroke: none,
    radius: 0pt,
    inset: (x: 0pt, top: 1.15pt, bottom: 1.15pt),
    number-format: n => text(size: 6.5pt, fill: hint, font: mono-font)[#str(n)#h(5.5pt)],
    number-align: right,
    number-placement: "inside",
  )

  // 章：不单独占整页，页首一个紧凑的标题带。
  show heading.where(level: 1): it => {
    block(above: 0pt, below: 14pt, width: 100%)[
      #set text(font: sans-font)
      #grid(
        columns: (1fr, auto),
        align: (left + horizon, right + horizon),
        text(size: 7.4pt, weight: "bold", fill: accent, tracking: 0.24em)[
          CHAPTER #context counter(heading).display("1")
        ],
        text(size: 7.4pt, fill: faint, tracking: 0.12em)[XCPC TEMPLATE],
      )
      #v(3pt)
      #text(size: 22pt, weight: "bold", fill: ink)[#it.body]
      #v(5pt)
      #line(length: 100%, stroke: 0.5pt + ink)
    ]
  }

  // 小节：编号 + 标题 + 复杂度，下面一条细线。
  show heading.where(level: 2): it => context {
    let marks = query(selector(metadata).before(it.location()))
    let found = marks.filter(mark => mark.value.kind == "cmplx")
    let value = if found.len() > 0 { found.last().value.value } else { none }
    block(above: 16pt, below: 8pt, width: 100%)[
      #grid(
        columns: (auto, 1fr, auto),
        column-gutter: 8pt,
        align: (left + horizon, left + horizon, right + horizon),
        text(font: sans-font, size: 8pt, weight: "bold", fill: accent, tracking: 0.02em)[
          #counter(heading).display("1.1")
        ],
        text(font: sans-font, size: 12.2pt, weight: "bold", fill: ink)[#it.body],
        if value == none {
          []
        } else {
          box(
            fill: accent-soft,
            inset: (x: 5pt, y: 2pt),
            radius: 2pt,
            text(font: sans-font, size: 7.2pt, fill: accent)[#value],
          )
        },
      )
      #v(3pt)
      #line(length: 100%, stroke: 0.4pt + rule)
    ]
  }

  // 封面
  page(header: none, footer: none, numbering: none, margin: (x: 22mm, top: 24mm, bottom: 20mm))[
    #set text(font: sans-font)
    #grid(
      columns: (1fr, auto),
      align: (left, right),
      text(size: 7.4pt, fill: accent, tracking: 0.26em)[XCPC TEMPLATE],
      text(size: 7.4pt, fill: muted, tracking: 0.08em)[#edition],
    )
    #v(3pt)
    #line(length: 100%, stroke: 0.6pt + ink)
    #v(34mm)
    #text(size: 62pt, weight: "bold", fill: accent, tracking: -0.02em)[XCPC]
    #v(4pt)
    #text(size: 24pt, weight: "bold", fill: ink)[#name]
    #v(10pt)
    #text(size: 10.2pt, fill: muted)[#subtitle]
    #v(8pt)
    #text(size: 8.6pt, fill: faint)[#count 个板子 · #chapters.len() 章 · A4 单面]

    #v(30mm)
    #text(size: 7.2pt, fill: accent, tracking: 0.22em)[CONTENTS]
    #v(4pt)
    #line(length: 30pt, stroke: 2.2pt + accent)
    #v(9pt)
    #grid(
      columns: (1fr, 1fr),
      column-gutter: 20pt,
      row-gutter: 9pt,
      ..chapters.enumerate().map(((i, name)) => grid(
        columns: (18pt, 1fr),
        column-gutter: 8pt,
        align: (right + horizon, left + horizon),
        text(size: 7.2pt, fill: faint, font: mono-font)[#(i + 1)],
        text(size: 9.2pt, fill: text-ink)[#name],
      )),
    )

    #place(bottom + left, block(width: 100%)[
      #text(size: 8.4pt, fill: muted)[先查字段，再抄代码；下标、复杂度、坑都写在代码前面。]
      #v(10pt)
      #line(length: 100%, stroke: 0.5pt + rule)
      #v(6pt)
      #grid(
        columns: (1fr, auto),
        text(size: 8pt, fill: muted)[#author],
        text(size: 8pt, fill: muted)[A4 · 现场赛用],
      )
    ])
  ]

  // 目录：两栏，尽量一页放下。
  page(header: none, footer: none, numbering: none, margin: (x: 20mm, top: 22mm, bottom: 20mm))[
    #set text(font: sans-font)
    #set par(leading: 0.4em, spacing: 0.45em)
    #grid(
      columns: (1fr, auto),
      align: (left, right),
      text(size: 22pt, weight: "bold", fill: ink)[目录],
      text(size: 7.4pt, fill: faint, tracking: 0.18em)[CONTENTS],
    )
    #v(5pt)
    #line(length: 100%, stroke: 0.6pt + ink)
    #v(10pt)
    #show outline.entry: set text(font: sans-font, size: 8.6pt, fill: text-ink)
    #show outline.entry.where(level: 1): it => {
      v(7pt, weak: true)
      set text(size: 9.8pt, weight: "bold", fill: ink)
      it
    }
    #outline(title: none, depth: 2, indent: 1.2em)
  ]

  counter(page).update(1)
  set page(
    numbering: "1",
    // duplex：内侧（装订边）20mm、外侧 16mm；单面回到对称边距。
    margin: if duplex {
      (inside: 20mm, outside: 16mm, top: 17mm, bottom: 17mm)
    } else {
      (left: 20mm, right: 17mm, top: 17mm, bottom: 17mm)
    },
    header: context {
      let here-page = here().page()
      let heads = query(heading.where(level: 1))
      let chapter = if heads.len() > 0 {
        let before = heads.filter(h => h.location().page() <= here-page)
        if before.len() > 0 { before.last().body } else { "" }
      } else {
        ""
      }
      let recto = not duplex or calc.odd(here-page)
      let left-text = if recto { short } else { chapter }
      let right-text = if recto { chapter } else { short }
      block(
        width: 100%,
        inset: (bottom: 5pt),
        stroke: (bottom: 0.3pt + rule),
      )[
        #set text(size: 6.8pt, font: sans-font, tracking: 0.04em)
        #grid(
          columns: (1fr, 1fr),
          align: (left + horizon, right + horizon),
          text(fill: faint)[#left-text],
          text(fill: muted)[#right-text],
        )
      ]
    },
    footer: context {
      let recto = not duplex or calc.odd(here().page())
      align(
        if recto { right } else { left },
        text(size: 7.4pt, font: sans-font, fill: muted)[#counter(page).display("1")],
      )
    },
  )

  body
}
