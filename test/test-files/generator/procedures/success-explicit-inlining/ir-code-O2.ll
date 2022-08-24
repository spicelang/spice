; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [21 x i8] c"This is a value: %d\0A\00", align 1
@str = private unnamed_addr constant [13 x i8] c"Before value\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"After value\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l5:
  %puts = tail call i32 @puts(ptr nonnull @str)
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 5)
  %puts1 = tail call i32 @puts(ptr nonnull @str.1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
