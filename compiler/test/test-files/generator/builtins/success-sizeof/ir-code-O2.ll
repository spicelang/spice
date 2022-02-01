; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@4 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"Hello Spice!\00", align 1
@6 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@intArray = dso_local constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
@7 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([17 x i8], [17 x i8]* @0, i64 0, i64 0), i32 32)
  %1 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([19 x i8], [19 x i8]* @1, i64 0, i64 0), i32 16)
  %2 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @2, i64 0, i64 0), i32 8)
  %3 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @3, i64 0, i64 0), i32 8)
  %4 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @4, i64 0, i64 0), i32 0)
  %5 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @6, i64 0, i64 0), i32 1)
  %6 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([19 x i8], [19 x i8]* @7, i64 0, i64 0), i32 7)
  ret i32 0
}
