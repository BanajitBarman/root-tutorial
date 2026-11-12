-- Lua filter used only for the PDF:
--  * program output (```text blocks) goes into a light grey box
--  * single plots are drawn at 75% of the text width, wide ones at full width
local function latex_escape_none(s) return s end

function CodeBlock(el)
  if el.classes[1] == "text" then
    local body = "\\begin{outputbox}\n\\begin{Verbatim}[breaklines,breakanywhere,fontsize=\\footnotesize]\n"
                 .. el.text .. "\n\\end{Verbatim}\n\\end{outputbox}"
    return pandoc.RawBlock("latex", body)
  end
end

function Image(el)
  if el.attributes.width then return el end
  local ok, contents = pcall(function()
    local fh = io.open(el.src, "rb")
    if not fh then return nil end
    local data = fh:read("a")
    fh:close()
    return data
  end)
  if ok and contents then
    local w, h = pandoc.image.size(contents)
    if w and h and w / h < 1.6 then
      el.attributes.width = "75%"
    else
      el.attributes.width = "100%"
    end
  end
  return el
end
