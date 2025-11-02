; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.D = type { %struct.A, %struct.C, i32 }
%struct.A = type { i32 }
%struct.C = type { %struct.A, %struct.B, i32 }
%struct.B = type { i32 }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"%d, %d, %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %d = alloca %struct.D, align 8
  store i32 0, ptr %result, align 4
  store %struct.D zeroinitializer, ptr %d, align 4
  %f1.addr = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 0, i32 0
  store i32 1, ptr %f1.addr, align 4
  %f2.addr = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 2
  store i32 2, ptr %f2.addr, align 4
  %f3.addr = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 1, i32 1, i32 0
  store i32 3, ptr %f3.addr, align 4
  %f1.addr1 = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 0, i32 0
  %1 = load i32, ptr %f1.addr1, align 4
  %f2.addr2 = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 2
  %2 = load i32, ptr %f2.addr2, align 4
  %f3.addr3 = getelementptr inbounds %struct.D, ptr %d, i64 0, i32 1, i32 1, i32 0
  %3 = load i32, ptr %f3.addr3, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1, i32 %2, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
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
