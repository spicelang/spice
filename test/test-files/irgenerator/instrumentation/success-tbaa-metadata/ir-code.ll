; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"i, j: %d, %d\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4, !type !5
  %i = alloca i32, align 4, !type !5
  %j = alloca i64, align 8, !type !6
  store i32 0, ptr %result, align 4, !tbaa !7
  store i32 123, ptr %i, align 4, !tbaa !7
  %1 = load i32, ptr %i, align 4, !tbaa !7
  %2 = zext i32 %1 to i64
  store i64 %2, ptr %j, align 8, !tbaa !11
  %3 = load i64, ptr %j, align 8, !tbaa !11
  %4 = add i64 %3, 1
  store i64 %4, ptr %j, align 8
  %5 = load i32, ptr %i, align 4, !tbaa !7
  %6 = load i64, ptr %j, align 8, !tbaa !11
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5, i64 noundef %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
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
!5 = !{i64 -5436590890822487957, !"int"}
!6 = !{i64 6042226155786013045, !"unsigned long"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !9, i64 0}
!9 = !{!"omnipotent byte", !10, i64 0}
!10 = !{!"Simple Spice TBAA"}
!11 = !{!12, !12, i64 0}
!12 = !{!"unsigned long", !9, i64 0}
