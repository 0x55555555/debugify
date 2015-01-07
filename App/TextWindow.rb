
module App

  module TextWindow
    SELECTED = "#e8e762"
    ALTERNATE_A = "#ffffff"
    ALTERNATE_B = "#f7f7f7"

    def alternatingColourList(list, options)
      rows = list.map.with_index do |t, i|
        col = nil
        if (i == options[:selected])
          col = SELECTED
        else
          col = i % 2 == 0 ? ALTERNATE_A : ALTERNATE_B
        end

        colouredRow(t, col)
      end

      return rows.join()
    end

    def colouredRow(str, col)
      return "<div style=\"background-color: #{col}; width: 100%\">#{str}</div>"
    end

    def link(href, str)
      return "<a href=\"#{href}\">#{str}<a>"
    end
  end

end