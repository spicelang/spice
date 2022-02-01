; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([13 x i8], [13 x i8]* @0, i64 0, i64 0))
  ret i32 0
}
