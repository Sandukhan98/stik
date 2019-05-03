#lang racket
(require xml)
(require web-server/servlet
         web-server/servlet-env)

;; An example of a page returning TEXT with calls to fprintf
(define (main-page req)
  (response/output
    #:mime-type TEXT/HTML-MIME-TYPE
    (lambda (out)
      (fprintf out "HELLO FROM THE OPEN MAPPING SERVICE~n")
      )))

;; An example of a page returning HTML with xexprs and macros
(define (display-page req)
  (response/xexpr
   `(html (head (title "OPEN MAPPING SERVICE DISPLAY PAGE"))
             (body
              (h1 ,(url->string (request-uri req)))
              (pre ,(format "~a" (request-bindings req)))))))

(define document-root
  (path->string (current-directory)))

(define (disp-ht req)
  ; extract the URI from the request:
  (define uri (request-uri req))
  
  ; extract the resource from the URI:
  (define resource 
    (map path/param-path (url-path uri)))
  
  ; find the file location:
  (define file (string-append
                document-root
                "/" 
                (string-join resource "/")))
  (cond
    
    ; serve the file if it exists:
    [(file-exists? file)
     ; =>
     
     ; read the file contents:
     (define data (file->bytes file))
     
     ; construct the response:
     (response/output
      #:mime-type TEXT/HTML-MIME-TYPE
      (λ (client-out)
        (write-bytes data client-out)))]
    
    ; send an error page otherwise:
    [else
     ; =>
     (response/xexpr
      #:code     404
      #:message  #"Not found"
      `(html
        (body
         (p "Not found"))))]))

(define (dispBrd-ht req)
  ; extract the URI from the request:
  (define uri (request-uri req))
  
  ; extract the resource from the URI:
  (define resource 
    (map path/param-path (url-path uri)))
  
  ; find the file location:
  (define file (string-append
                document-root
                "/" 
                (string-join resource "/")))
  (cond
    
    ; serve the file if it exists:
    [(file-exists? file)
     ; =>
     
     ; read the file contents:
     (define data (file->bytes file))
     
     ; construct the response:
     (response/output
      #:mime-type TEXT/HTML-MIME-TYPE
      (λ (client-out)
        (write-bytes data client-out)))]
    
    ; send an error page otherwise:
    [else
     ; =>
     (response/xexpr
      #:code     404
      #:message  #"Not found"
      `(html
        (body
         (p "Not found"))))]))

(define (disp-js req)
  ; extract the URI from the request:
  (define uri (request-uri req))
  
  ; extract the resource from the URI:
  (define resource 
    (map path/param-path (url-path uri)))
  
  ; find the file location:
  (define file (string-append
                document-root
                "/" 
                (string-join resource "/")))
  (cond
    
    ; serve the file if it exists:
    [(file-exists? file)
     ; =>
     
     ; read the file contents:
     (define data (file->bytes file))
     
     ; construct the response:
     (response/output
   #:mime-type #"application/javascript"
      (λ (client-out)
        (write-bytes data client-out)))]
    
    ; send an error page otherwise:
    [else
     ; =>
     (response/xexpr
      #:code     404
      #:message  #"Not found"
      `(html
        (body
         (p "Not found"))))]))

(define (dispBrd-js req)
  ; extract the URI from the request:
  (define uri (request-uri req))
  
  ; extract the resource from the URI:
  (define resource 
    (map path/param-path (url-path uri)))
  
  ; find the file location:
  (define file (string-append
                document-root
                "/" 
                (string-join resource "/")))
  (cond
    
    ; serve the file if it exists:
    [(file-exists? file)
     ; =>
     
     ; read the file contents:
     (define data (file->bytes file))
     
     ; construct the response:
     (response/output
   #:mime-type #"application/javascript"
      (λ (client-out)
        (write-bytes data client-out)))]
    
    ; send an error page otherwise:
    [else
     ; =>
     (response/xexpr
      #:code     404
      #:message  #"Not found"
      `(html
        (body
         (p "Not found"))))]))

;; Routing function
;;     /display          --->   display-page
;;     everything else   --->   main-page
(define-values (server-dispatch server-url)
    (dispatch-rules
     [("display") display-page]
     [("test.html") disp-ht]
     [("board.html") dispBrd-ht]
     [("data.json") disp-js]
     [("dataBrd.json") dispBrd-js]
     [else main-page]))

(serve/servlet server-dispatch
               #:servlet-regexp #rx""
               #:port 9000
               #:launch-browser? #f)