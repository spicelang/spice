; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
for.body.L5.preheader.3:
  %0 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 0)
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 2)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 4)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 6)
  br label %for.body.L5.3

for.body.L5.3:                                    ; preds = %for.body.L5.3, %for.body.L5.preheader.3
  %subCounter.07.3 = phi i32 [ %5, %for.body.L5.3 ], [ 100, %for.body.L5.preheader.3 ]
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 %subCounter.07.3)
  %5 = add nsw i32 %subCounter.07.3, -1
  %6 = icmp ugt i32 %subCounter.07.3, 11
  br i1 %6, label %for.body.L5.3, label %for.body.L5.preheader.4

for.body.L5.preheader.4:                          ; preds = %for.body.L5.3
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 8)
  br label %for.body.L5.4

for.body.L5.4:                                    ; preds = %for.body.L5.4, %for.body.L5.preheader.4
  %subCounter.07.4 = phi i32 [ %9, %for.body.L5.4 ], [ 100, %for.body.L5.preheader.4 ]
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 %subCounter.07.4)
  %9 = add nsw i32 %subCounter.07.4, -1
  %10 = icmp ugt i32 %subCounter.07.4, 11
  br i1 %10, label %for.body.L5.4, label %for.tail.L2.4

for.tail.L2.4:                                    ; preds = %for.body.L5.4
  %11 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
