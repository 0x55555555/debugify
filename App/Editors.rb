require 'set'

module App

  class Editors
    def initialize(mainwindow, debugger, project)

      @mainwindow = mainwindow
      @debugger = debugger
      @editors = { }
      @activeEditors = [ ]
      @pendingTypes = Set.new()

      mainwindow.editorOpened.listen { |editor| setupEditor(editor) }
      mainwindow.editorClosed.listen { |editor| destroyEditor(editor) }
      mainwindow.typeAdded.listen { |t| typeDiscovered(t) }
      project.install_handler(:editors, self)
    end

    def owns_value(id)
      return id.to_sym == :editors
    end

    def deserialise(handler)
      editors = handler.value(:editors) || []

      editors.each do |e|
        path = e["path"]
        type = e["type"]
        if (path == nil || type == nil)
          next
        end

        if (type == "file")
          @mainwindow.openFile(path, 0)
        else
          if (@mainwindow.openType(path) == nil)
            @pendingTypes << path
          end
        end
      end
    end

    def serialise(handler)
      editors = []
      @editors.each { |k, v|
        editors <<  {
          "path" => k,
          "type" => v.is_a?(UI::FileEditor) ? "file" : "type",
        }
      }

      if (handler.has_location(:exe))
        handler.set_value(:editors, editors, :exe)
      end
    end

    def syncEditorBreakpoints()
      @editors.each do |_, e|
        if (e.is_a? UI::FileEditor)
          e.clearMarkers(UI::FileEditor::MarkerType[:Breakpoint]) 
        end
      end

      target = @debugger.target
      target.breakpoints.each do |br|
        br.locations.each do |l|
          addEditorBreakpoint(l.file, l.line)
        end
      end
    end

    def loadCurrentSource(process)
      thread = process.selectedThread
      frame = thread.selectedFrame

      file = frame.filename
      line = frame.lineNumber
      if (file.length > 0)
        editor = @mainwindow.openFile(file, line)
      end
    end

    def clearSourceHighlight()
      @activeEditors.each do |e|
        e.clearMarkers(UI::FileEditor::MarkerType[:ActiveLine])
        e.clearMarkers(UI::FileEditor::MarkerType[:CurrentLine])
      end
      @activeEditors.clear()
    end

    def highlightSources()
      clearSourceHighlight()

      if (@mainwindow.process == nil)
        return
      end
      
      @mainwindow.process.threads.each do |t|
        frame = t.selectedFrame

        if (frame.hasLineNumber)
          current = t.isCurrent

          line = frame.lineNumber
          file =  frame.filename

          editor = @editors[file]
          if (editor)
            @activeEditors << editor
            type = current ? UI::FileEditor::MarkerType[:CurrentLine] :
              UI::FileEditor::MarkerType[:ActiveLine]
            editor.addMarker(type, line)
          end
        end
      end
    end

  private
    def typeDiscovered(type)
      if (@pendingTypes.delete?(type) != nil)
        raise "Failed to open #{type}" unless @mainwindow.openType(type)
      end
    end

    def setupEditor(editor)
      if (editor.class == UI::FileEditor)
        setupFileEditor(editor)
      end

      @editors[editor.path] = editor
    end

    def destroyEditor(editor)
      @editors.delete(editor.path)
    end

    def setupFileEditor(editor)
      path = editor.path()
      editor.marginClicked.listen do |line|
        found, brk, loc = @mainwindow.target.findBreakpoint(path, line)
        if (found)
          @mainwindow.target.removeBreakpoint(brk)
        else
          @mainwindow.target.addBreakpoint(path, line)
        end
      end

      highlightSources()
      syncEditorBreakpoints()
    end

    def addEditorBreakpoint(file, line)
      editor = @editors[file]

      editor.addMarker(UI::FileEditor::MarkerType[:Breakpoint], line) if editor
    end
  end

end