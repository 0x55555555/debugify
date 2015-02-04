require_relative 'Utils'

module App
  class ProcessSelector
    def self.show(proc = nil)
      procs, closest = self.processes(proc)

      dlg = UI::Dialog.new(UI::UIC_PATH + "processselector.ui")

      dlg.changed.listen do |p|
        if (p == "processes")
          val = dlg.value(p)
          dlg.setValue("pid", procs.key(val))
        end
      end

      dlg.setAvailableValues("processes", procs.values)
      if (closest)
        dlg.setValue("processes", procs[closest])
        dlg.setValue("pid", closest)
      end

      dlg.exec()
      if (UI::Dialog::Result[dlg.result()] != :Accepted)
        return nil
      end

      val = dlg.value("pid")
      return val.to_i
    end

    def self.processes(search)
      processes = (`ps -x -o pid -o comm`).split("\n")
      processes.shift

      elements = processes.map do |x| 
        y = x.split
        
        next [ y[0], y[1..(y.length-1)].join(" ") ].flatten
      end

      closest = nil
      strings = { }
      elements.each do |e| 
        bn = File.basename(e[1])
        short = App.shortPath(e[1])
        if (short == bn)
          short = ""
        else
          short = " (#{short})"
        end

        id = e[0].to_i
        if (bn == search && !closest)
          closest = id
        end

        strings[id] = "#{id} #{bn}#{short}" 
      end

      strings = strings.sort.reverse
      strings.shift

      return strings.to_h, closest
    end
  end
end