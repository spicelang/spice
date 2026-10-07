; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Large = type { i64, i64, i64, i64 }
%struct.Point = type { double, double }
%struct.Triple = type { i32, i32, i32 }

@printf.str.0 = private unnamed_addr constant [28 x i8] c"%f %f %d %d %d %d %d %d %d\0A\00", align 4

; Function Attrs: nounwind
declare { double, double } @getOrigin() #0

; Function Attrs: nounwind
declare { i64, i32 } @getTriple() #0

; Function Attrs: nounwind
declare void @getLarge(ptr dead_on_unwind noalias writable sret(%struct.Large) align 8, i64 noundef) #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %point = alloca %struct.Point, align 8
  %triple = alloca { i64, i32 }, align 8
  %large = alloca %struct.Large, align 8
  %1 = call { double, double } @getOrigin()
  store { double, double } %1, ptr %point, align 8
  %2 = call { i64, i32 } @getTriple()
  store { i64, i32 } %2, ptr %triple, align 8
  call void @getLarge(ptr dead_on_unwind writable sret(%struct.Large) align 8 %large, i64 noundef 10)
  %x.addr = getelementptr inbounds %struct.Point, ptr %point, i64 0, i32 0
  %3 = load double, ptr %x.addr, align 8
  %y.addr = getelementptr inbounds %struct.Point, ptr %point, i64 0, i32 1
  %4 = load double, ptr %y.addr, align 8
  %a.addr = getelementptr inbounds %struct.Triple, ptr %triple, i64 0, i32 0
  %5 = load i32, ptr %a.addr, align 4
  %b.addr = getelementptr inbounds %struct.Triple, ptr %triple, i64 0, i32 1
  %6 = load i32, ptr %b.addr, align 4
  %c.addr = getelementptr inbounds %struct.Triple, ptr %triple, i64 0, i32 2
  %7 = load i32, ptr %c.addr, align 4
  %a.addr1 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 0
  %8 = load i64, ptr %a.addr1, align 8
  %b.addr2 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 1
  %9 = load i64, ptr %b.addr2, align 8
  %c.addr3 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 2
  %10 = load i64, ptr %c.addr3, align 8
  %d.addr = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 3
  %11 = load i64, ptr %d.addr, align 8
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double noundef %3, double noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, i64 noundef %8, i64 noundef %9, i64 noundef %10, i64 noundef %11)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
