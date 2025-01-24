; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
for.body.L5.preheader.3:
  %0 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 0)
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 2)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 4)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 6)
  br label %for.body.L5.3

for.body.L5.3:                                    ; preds = %for.body.L5.3, %for.body.L5.preheader.3
  %subCounter.07.3 = phi i32 [ %5, %for.body.L5.3 ], [ 100, %for.body.L5.preheader.3 ]
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %subCounter.07.3)
  %5 = add nsw i32 %subCounter.07.3, -1
  %or.cond.3 = icmp ugt i32 %subCounter.07.3, 11
  br i1 %or.cond.3, label %for.body.L5.3, label %for.body.L5.preheader.4

for.body.L5.preheader.4:                          ; preds = %for.body.L5.3
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 8)
  br label %for.body.L5.4

for.body.L5.4:                                    ; preds = %for.body.L5.4, %for.body.L5.preheader.4
  %subCounter.07.4 = phi i32 [ %8, %for.body.L5.4 ], [ 100, %for.body.L5.preheader.4 ]
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %subCounter.07.4)
  %8 = add nsw i32 %subCounter.07.4, -1
  %or.cond.4 = icmp ugt i32 %subCounter.07.4, 11
  br i1 %or.cond.4, label %for.body.L5.4, label %for.tail.L2.4

for.tail.L2.4:                                    ; preds = %for.body.L5.4
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
