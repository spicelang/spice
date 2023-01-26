; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %for.body.L2

for.body.L2:                                      ; preds = %for.body.L2, %0
  %counter.09 = phi i32 [ 0, %0 ], [ %3, %for.body.L2 ]
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %counter.09)
  %2 = icmp ugt i32 %counter.09, 9000
  %3 = add nuw nsw i32 %counter.09, 2
  br i1 %2, label %for.body.L5, label %for.body.L2

for.body.L5:                                      ; preds = %for.body.L2, %for.body.L5
  %subCounter.08 = phi i32 [ %6, %for.body.L5 ], [ 100, %for.body.L2 ]
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %subCounter.08)
  %5 = icmp eq i32 %subCounter.08, 11
  %6 = add nsw i32 %subCounter.08, -1
  br i1 %5, label %for.exit.L2, label %for.body.L5

for.exit.L2:                                      ; preds = %for.body.L5
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
