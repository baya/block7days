Once you have a tags file and M-x visit-tags-table, you can follow tags (of functions, variables, macros, whatever) to their definitions. These are the basic commands:

`M-.’ (‘find-tag’) – find a tag, that is, use the Tags file to look up a definition. If there are multiple tags in the project with the same name, use `C-u M-.’ to go to the next match.
‘M-x find-tag-other-window’ – selects the buffer containing the tag’s definition in another window, and move point there.
‘M-*’ (‘pop-tag-mark’) – jump back
‘M-x tags-search’ – regexp-search through the source files indexed by a tags file (a bit like ‘grep’)
‘M-x tags-query-replace’ – query-replace through the source files indexed by a tags file
`M-,’ (‘tags-loop-continue’) – resume ‘tags-search’ or ‘tags-query-replace’ starting at point in a source file
‘M-x tags-apropos’ – list all tags in a tags file that match a regexp
‘M-x list-tags’ – list all tags defined in a source file

From within Emacs
Put this in your init file:

  (setq path-to-ctags "/opt/local/bin/ctags") ;; <- your ctags path here
  (defun create-tags (dir-name)
    "Create tags file."
    (interactive "DDirectory: ")
    (shell-command
     (format "%s -f TAGS -e -R %s" path-to-ctags (directory-file-name dir-name)))
  )
  
  
  cd /path/to/my/project
  ctags -e -R .
You can filter it to specific extensions if needed:

  cd /path/to/my/project
  ctags -e -R *.cpp *.hpp *.h
