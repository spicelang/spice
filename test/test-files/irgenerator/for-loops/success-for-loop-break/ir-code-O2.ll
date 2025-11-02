; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  br label %for.body.L2

for.body.L2:                                      ; preds = %for.body.L2, %0
  %counter.09 = phi i32 [ 0, %0 ], [ %3, %for.body.L2 ]
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %counter.09)
  %2 = icmp samesign ugt i32 %counter.09, 9000
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
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
