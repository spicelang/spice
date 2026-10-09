; ModuleID = 'source.spice'
source_filename = "source.spice"

%union.Value = type { i32, [0 x double], [8 x i8] }
%struct.Vec2 = type { i32, i32 }
%union.Box = type { i32, [0 x i64], [8 x i8] }
%union.Box.0 = type { i32, [0 x double], [8 x i8] }
%struct.Holder = type { %union.Value, %union.Box.1 }
%union.Box.1 = type { i32, [0 x i64], [8 x i8] }

@anon.string.0 = private unnamed_addr constant [93 x i8] c"Program panicked at ./source.spice:28:12: active field mismatch on union field access 'vec'\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [93 x i8] c"Program panicked at ./source.spice:28:26: active field mismatch on union field access 'vec'\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 4, !dbg !0
@anon.string.2 = private unnamed_addr constant [91 x i8] c"Program panicked at ./source.spice:34:25: active field mismatch on union field access 'i'\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Double: %f\0A\00", align 4, !dbg !5
@anon.string.3 = private unnamed_addr constant [91 x i8] c"Program panicked at ./source.spice:36:28: active field mismatch on union field access 'd'\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4, !dbg !9
@printf.str.3 = private unnamed_addr constant [15 x i8] c"Boxes: %d, %f\0A\00", align 4, !dbg !12
@anon.string.4 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:44:31: active field mismatch on union field access 'value'\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:44:45: active field mismatch on union field access 'value'\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [19 x i8] c"Fallbacks: %d, %d\0A\00", align 4, !dbg !15
@anon.string.6 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:47:35: active field mismatch on union field access 'fallback'\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:47:52: active field mismatch on union field access 'fallback'\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [18 x i8] c"Holder value: %d\0A\00", align 4, !dbg !18
@anon.string.8 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:51:34: active field mismatch on union field access 'value'\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [21 x i8] c"Holder fallback: %d\0A\00", align 4, !dbg !21
@anon.string.9 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:54:37: active field mismatch on union field access 'fallback'\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z3sumRK5Value(ptr noundef %0) #0 !dbg !32 {
  %value = alloca ptr, align 8
    #dbg_declare(ptr %value, !51, !DIExpression(), !52)
  store ptr %0, ptr %value, align 8, !dbg !52
  %2 = load ptr, ptr %value, align 8, !dbg !53
  %3 = load i32, ptr %2, align 4, !dbg !53
  %4 = icmp eq i32 %3, 3, !dbg !53
  br i1 %4, label %union.tag.ok.L28, label %union.tag.panic.L28, !dbg !53, !prof !54

union.tag.panic.L28:                              ; preds = %1
  %5 = call ptr @__acrt_iob_func(i32 2), !dbg !53
  %6 = call i32 (ptr, ptr, ...) @fprintf(ptr %5, ptr @anon.string.0), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

union.tag.ok.L28:                                 ; preds = %1
  %vec.addr = getelementptr inbounds nuw %union.Value, ptr %2, i32 0, i32 2, !dbg !53
  %x.addr = getelementptr inbounds %struct.Vec2, ptr %vec.addr, i64 0, i32 0, !dbg !53
  %7 = load ptr, ptr %value, align 8, !dbg !55
  %8 = load i32, ptr %7, align 4, !dbg !55
  %9 = icmp eq i32 %8, 3, !dbg !55
  br i1 %9, label %union.tag.ok.L282, label %union.tag.panic.L281, !dbg !55, !prof !54

union.tag.panic.L281:                             ; preds = %union.tag.ok.L28
  %10 = call ptr @__acrt_iob_func(i32 2), !dbg !55
  %11 = call i32 (ptr, ptr, ...) @fprintf(ptr %10, ptr @anon.string.1), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

union.tag.ok.L282:                                ; preds = %union.tag.ok.L28
  %vec.addr3 = getelementptr inbounds nuw %union.Value, ptr %7, i32 0, i32 2, !dbg !55
  %y.addr = getelementptr inbounds %struct.Vec2, ptr %vec.addr3, i64 0, i32 1, !dbg !55
  %12 = load i32, ptr %y.addr, align 4, !dbg !55
  %13 = load i32, ptr %x.addr, align 4, !dbg !55
  %14 = add nsw i32 %13, %12, !dbg !55
  ret i32 %14, !dbg !56
}

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #1

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #4 !dbg !57 {
  %value = alloca %union.Value, align 8
  %intBox = alloca %union.Box, align 8
  %doubleBox = alloca %union.Box.0, align 8
  %holder = alloca %struct.Holder, align 8
    #dbg_declare(ptr %value, !60, !DIExpression(), !61)
  store %union.Value zeroinitializer, ptr %value, align 8, !dbg !61
  store i32 1, ptr %value, align 4, !dbg !62
  %i.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !62
  store i32 42, ptr %i.addr, align 4, !dbg !63
  %1 = load i32, ptr %value, align 4, !dbg !64
  %2 = icmp eq i32 %1, 1, !dbg !64
  br i1 %2, label %union.tag.ok.L34, label %union.tag.panic.L34, !dbg !64, !prof !54

union.tag.panic.L34:                              ; preds = %0
  %3 = call ptr @__acrt_iob_func(i32 2), !dbg !64
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.2), !dbg !64
  call void @exit(i32 1), !dbg !64
  unreachable, !dbg !64

union.tag.ok.L34:                                 ; preds = %0
  %i.addr1 = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !64
  %5 = load i32, ptr %i.addr1, align 4, !dbg !64
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5), !dbg !64
  store i32 2, ptr %value, align 4, !dbg !65
  %d.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !65
  store double 3.500000e+00, ptr %d.addr, align 8, !dbg !66
  %7 = load i32, ptr %value, align 4, !dbg !67
  %8 = icmp eq i32 %7, 2, !dbg !67
  br i1 %8, label %union.tag.ok.L36, label %union.tag.panic.L36, !dbg !67, !prof !54

union.tag.panic.L36:                              ; preds = %union.tag.ok.L34
  %9 = call ptr @__acrt_iob_func(i32 2), !dbg !67
  %10 = call i32 (ptr, ptr, ...) @fprintf(ptr %9, ptr @anon.string.3), !dbg !67
  call void @exit(i32 1), !dbg !67
  unreachable, !dbg !67

union.tag.ok.L36:                                 ; preds = %union.tag.ok.L34
  %d.addr2 = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !67
  %11 = load double, ptr %d.addr2, align 8, !dbg !67
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, double noundef %11), !dbg !67
  store i32 3, ptr %value, align 4, !dbg !68
  %vec.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !68
  store %struct.Vec2 { i32 1, i32 2 }, ptr %vec.addr, align 4, !dbg !69
  %13 = call noundef i32 @_Z3sumRK5Value(ptr noundef %value), !dbg !70
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %13), !dbg !70
    #dbg_declare(ptr %intBox, !71, !DIExpression(), !77)
  store %union.Box zeroinitializer, ptr %intBox, align 8, !dbg !77
  store i32 1, ptr %intBox, align 4, !dbg !78
  %value.addr = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !78
  store i32 7, ptr %value.addr, align 4, !dbg !79
    #dbg_declare(ptr %doubleBox, !80, !DIExpression(), !85)
  store %union.Box.0 zeroinitializer, ptr %doubleBox, align 8, !dbg !85
  store i32 1, ptr %doubleBox, align 4, !dbg !86
  %value.addr3 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !86
  store double 2.500000e+00, ptr %value.addr3, align 8, !dbg !87
  %15 = load i32, ptr %intBox, align 4, !dbg !88
  %16 = icmp eq i32 %15, 1, !dbg !88
  br i1 %16, label %union.tag.ok.L44, label %union.tag.panic.L44, !dbg !88, !prof !54

union.tag.panic.L44:                              ; preds = %union.tag.ok.L36
  %17 = call ptr @__acrt_iob_func(i32 2), !dbg !88
  %18 = call i32 (ptr, ptr, ...) @fprintf(ptr %17, ptr @anon.string.4), !dbg !88
  call void @exit(i32 1), !dbg !88
  unreachable, !dbg !88

union.tag.ok.L44:                                 ; preds = %union.tag.ok.L36
  %value.addr4 = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !88
  %19 = load i32, ptr %value.addr4, align 4, !dbg !88
  %20 = load i32, ptr %doubleBox, align 4, !dbg !89
  %21 = icmp eq i32 %20, 1, !dbg !89
  br i1 %21, label %union.tag.ok.L446, label %union.tag.panic.L445, !dbg !89, !prof !54

union.tag.panic.L445:                             ; preds = %union.tag.ok.L44
  %22 = call ptr @__acrt_iob_func(i32 2), !dbg !89
  %23 = call i32 (ptr, ptr, ...) @fprintf(ptr %22, ptr @anon.string.5), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

union.tag.ok.L446:                                ; preds = %union.tag.ok.L44
  %value.addr7 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !89
  %24 = load double, ptr %value.addr7, align 8, !dbg !89
  %25 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %19, double noundef %24), !dbg !89
  store i32 2, ptr %intBox, align 4, !dbg !90
  %fallback.addr = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !90
  store i64 8, ptr %fallback.addr, align 8, !dbg !91
  store i32 2, ptr %doubleBox, align 4, !dbg !92
  %fallback.addr8 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !92
  store i64 9, ptr %fallback.addr8, align 8, !dbg !93
  %26 = load i32, ptr %intBox, align 4, !dbg !94
  %27 = icmp eq i32 %26, 2, !dbg !94
  br i1 %27, label %union.tag.ok.L47, label %union.tag.panic.L47, !dbg !94, !prof !54

union.tag.panic.L47:                              ; preds = %union.tag.ok.L446
  %28 = call ptr @__acrt_iob_func(i32 2), !dbg !94
  %29 = call i32 (ptr, ptr, ...) @fprintf(ptr %28, ptr @anon.string.6), !dbg !94
  call void @exit(i32 1), !dbg !94
  unreachable, !dbg !94

union.tag.ok.L47:                                 ; preds = %union.tag.ok.L446
  %fallback.addr9 = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !94
  %30 = load i64, ptr %fallback.addr9, align 8, !dbg !94
  %31 = load i32, ptr %doubleBox, align 4, !dbg !95
  %32 = icmp eq i32 %31, 2, !dbg !95
  br i1 %32, label %union.tag.ok.L4711, label %union.tag.panic.L4710, !dbg !95, !prof !54

union.tag.panic.L4710:                            ; preds = %union.tag.ok.L47
  %33 = call ptr @__acrt_iob_func(i32 2), !dbg !95
  %34 = call i32 (ptr, ptr, ...) @fprintf(ptr %33, ptr @anon.string.7), !dbg !95
  call void @exit(i32 1), !dbg !95
  unreachable, !dbg !95

union.tag.ok.L4711:                               ; preds = %union.tag.ok.L47
  %fallback.addr12 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !95
  %35 = load i64, ptr %fallback.addr12, align 8, !dbg !95
  %36 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i64 noundef %30, i64 noundef %35), !dbg !95
    #dbg_declare(ptr %holder, !96, !DIExpression(), !106)
  store %struct.Holder zeroinitializer, ptr %holder, align 8, !dbg !106
  %box.addr = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !107
  store i32 1, ptr %box.addr, align 4, !dbg !107
  %value.addr13 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr, i32 0, i32 2, !dbg !107
  store i16 12, ptr %value.addr13, align 2, !dbg !108
  %box.addr14 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !109
  %37 = load i32, ptr %box.addr14, align 4, !dbg !109
  %38 = icmp eq i32 %37, 1, !dbg !109
  br i1 %38, label %union.tag.ok.L51, label %union.tag.panic.L51, !dbg !109, !prof !54

union.tag.panic.L51:                              ; preds = %union.tag.ok.L4711
  %39 = call ptr @__acrt_iob_func(i32 2), !dbg !109
  %40 = call i32 (ptr, ptr, ...) @fprintf(ptr %39, ptr @anon.string.8), !dbg !109
  call void @exit(i32 1), !dbg !109
  unreachable, !dbg !109

union.tag.ok.L51:                                 ; preds = %union.tag.ok.L4711
  %value.addr15 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr14, i32 0, i32 2, !dbg !109
  %41 = load i16, ptr %value.addr15, align 2, !dbg !109
  %42 = sext i16 %41 to i32, !dbg !109
  %43 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %42), !dbg !109
  %box.addr16 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !110
  store i32 2, ptr %box.addr16, align 4, !dbg !110
  %fallback.addr17 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr16, i32 0, i32 2, !dbg !110
  store i64 123, ptr %fallback.addr17, align 8, !dbg !111
  %value.addr18 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 0, !dbg !112
  store i32 4, ptr %value.addr18, align 4, !dbg !112
  %next.addr = getelementptr inbounds nuw %union.Value, ptr %value.addr18, i32 0, i32 2, !dbg !112
  store ptr %value, ptr %next.addr, align 8, !dbg !113
  %box.addr19 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !114
  %44 = load i32, ptr %box.addr19, align 4, !dbg !114
  %45 = icmp eq i32 %44, 2, !dbg !114
  br i1 %45, label %union.tag.ok.L54, label %union.tag.panic.L54, !dbg !114, !prof !54

union.tag.panic.L54:                              ; preds = %union.tag.ok.L51
  %46 = call ptr @__acrt_iob_func(i32 2), !dbg !114
  %47 = call i32 (ptr, ptr, ...) @fprintf(ptr %46, ptr @anon.string.9), !dbg !114
  call void @exit(i32 1), !dbg !114
  unreachable, !dbg !114

union.tag.ok.L54:                                 ; preds = %union.tag.ok.L51
  %fallback.addr20 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr19, i32 0, i32 2, !dbg !114
  %48 = load i64, ptr %fallback.addr20, align 8, !dbg !114
  %49 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i64 noundef %48), !dbg !114
  ret i32 0, !dbg !115
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
attributes #2 = { nofree }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #5 = { nofree nounwind }

!llvm.module.flags = !{!25, !26, !27, !28, !29, !30}
!llvm.ident = !{!31}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !7, line: 34, type: !24, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [host] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/test/./test-files/irgenerator/instrumentation/success-dbg-info-union/source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-union")
!4 = !{!0, !5, !9, !12, !15, !18, !21}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !7, line: 36, type: !8, isLocal: true, isDefinition: true)
!7 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-union")
!8 = !DIStringType(name: "printf.str.1", size: 96)
!9 = !DIGlobalVariableExpression(var: !10, expr: !DIExpression())
!10 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !7, line: 38, type: !11, isLocal: true, isDefinition: true)
!11 = !DIStringType(name: "printf.str.2", size: 72)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(name: "printf.str.3", linkageName: "printf.str.3", scope: !2, file: !7, line: 44, type: !14, isLocal: true, isDefinition: true)
!14 = !DIStringType(name: "printf.str.3", size: 120)
!15 = !DIGlobalVariableExpression(var: !16, expr: !DIExpression())
!16 = distinct !DIGlobalVariable(name: "printf.str.4", linkageName: "printf.str.4", scope: !2, file: !7, line: 47, type: !17, isLocal: true, isDefinition: true)
!17 = !DIStringType(name: "printf.str.4", size: 152)
!18 = !DIGlobalVariableExpression(var: !19, expr: !DIExpression())
!19 = distinct !DIGlobalVariable(name: "printf.str.5", linkageName: "printf.str.5", scope: !2, file: !7, line: 51, type: !20, isLocal: true, isDefinition: true)
!20 = !DIStringType(name: "printf.str.5", size: 144)
!21 = !DIGlobalVariableExpression(var: !22, expr: !DIExpression())
!22 = distinct !DIGlobalVariable(name: "printf.str.6", linkageName: "printf.str.6", scope: !2, file: !7, line: 54, type: !23, isLocal: true, isDefinition: true)
!23 = !DIStringType(name: "printf.str.6", size: 168)
!24 = !DIStringType(name: "printf.str.0", size: 72)
!25 = !{i32 8, !"PIC Level", i32 2}
!26 = !{i32 7, !"PIE Level", i32 2}
!27 = !{i32 7, !"uwtable", i32 2}
!28 = !{i32 7, !"frame-pointer", i32 0}
!29 = !{i32 7, !"Dwarf Version", i32 5}
!30 = !{i32 2, !"Debug Info Version", i32 3}
!31 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
!32 = distinct !DISubprogram(name: "sum", linkageName: "_Z3sumRK5Value", scope: !7, file: !7, line: 27, type: !33, scopeLine: 27, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !50)
!33 = !DISubroutineType(types: !34)
!34 = !{!35, !36}
!35 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!36 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !37, size: 64)
!37 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !38)
!38 = !DICompositeType(tag: DW_TAG_union_type, name: "Value", scope: !7, file: !7, line: 10, size: 128, align: 64, elements: !39, identifier: "union.Value")
!39 = !{!40, !41, !43, !48}
!40 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !38, file: !7, line: 11, baseType: !35, size: 32, offset: 64)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "d", scope: !38, file: !7, line: 12, baseType: !42, size: 64, offset: 64)
!42 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!43 = !DIDerivedType(tag: DW_TAG_member, name: "vec", scope: !38, file: !7, line: 13, baseType: !44, size: 64, align: 32, offset: 64)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vec2", scope: !7, file: !7, line: 5, size: 64, align: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !45, identifier: "struct.Vec2")
!45 = !{!46, !47}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !44, file: !7, line: 6, baseType: !35, size: 32)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !44, file: !7, line: 7, baseType: !35, size: 32, offset: 32)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "next", scope: !38, file: !7, line: 14, baseType: !49, size: 64, offset: 64)
!49 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !38, size: 64)
!50 = !{}
!51 = !DILocalVariable(name: "value", arg: 1, scope: !32, file: !7, line: 27, type: !36)
!52 = !DILocation(line: 27, column: 12, scope: !32)
!53 = !DILocation(line: 28, column: 12, scope: !32)
!54 = !{!"branch_weights", i32 1048575, i32 1}
!55 = !DILocation(line: 28, column: 26, scope: !32)
!56 = !DILocation(line: 29, column: 1, scope: !32)
!57 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 31, type: !58, scopeLine: 31, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !50)
!58 = !DISubroutineType(types: !59)
!59 = !{!35}
!60 = !DILocalVariable(name: "value", scope: !57, file: !7, line: 32, type: !38)
!61 = !DILocation(line: 32, column: 5, scope: !57)
!62 = !DILocation(line: 33, column: 5, scope: !57)
!63 = !DILocation(line: 33, column: 15, scope: !57)
!64 = !DILocation(line: 34, column: 25, scope: !57)
!65 = !DILocation(line: 35, column: 5, scope: !57)
!66 = !DILocation(line: 35, column: 15, scope: !57)
!67 = !DILocation(line: 36, column: 28, scope: !57)
!68 = !DILocation(line: 37, column: 5, scope: !57)
!69 = !DILocation(line: 37, column: 26, scope: !57)
!70 = !DILocation(line: 38, column: 29, scope: !57)
!71 = !DILocalVariable(name: "intBox", scope: !57, file: !7, line: 40, type: !72)
!72 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<int>", scope: !7, file: !7, line: 17, size: 128, align: 64, elements: !73, identifier: "union.Box")
!73 = !{!74, !75}
!74 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !72, file: !7, line: 18, baseType: !35, size: 32, offset: 64)
!75 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !72, file: !7, line: 19, baseType: !76, size: 64, offset: 64)
!76 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!77 = !DILocation(line: 40, column: 5, scope: !57)
!78 = !DILocation(line: 41, column: 5, scope: !57)
!79 = !DILocation(line: 41, column: 20, scope: !57)
!80 = !DILocalVariable(name: "doubleBox", scope: !57, file: !7, line: 42, type: !81)
!81 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<double>", scope: !7, file: !7, line: 17, size: 128, align: 64, elements: !82, identifier: "union.Box")
!82 = !{!83, !84}
!83 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !81, file: !7, line: 18, baseType: !42, size: 64, offset: 64)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !81, file: !7, line: 19, baseType: !76, size: 64, offset: 64)
!85 = !DILocation(line: 42, column: 5, scope: !57)
!86 = !DILocation(line: 43, column: 5, scope: !57)
!87 = !DILocation(line: 43, column: 23, scope: !57)
!88 = !DILocation(line: 44, column: 31, scope: !57)
!89 = !DILocation(line: 44, column: 45, scope: !57)
!90 = !DILocation(line: 45, column: 5, scope: !57)
!91 = !DILocation(line: 45, column: 23, scope: !57)
!92 = !DILocation(line: 46, column: 5, scope: !57)
!93 = !DILocation(line: 46, column: 26, scope: !57)
!94 = !DILocation(line: 47, column: 35, scope: !57)
!95 = !DILocation(line: 47, column: 52, scope: !57)
!96 = !DILocalVariable(name: "holder", scope: !57, file: !7, line: 49, type: !97)
!97 = !DICompositeType(tag: DW_TAG_structure_type, name: "Holder", scope: !7, file: !7, line: 22, size: 256, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !98, identifier: "struct.Holder")
!98 = !{!99, !100}
!99 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !97, file: !7, line: 23, baseType: !38, size: 128, align: 64)
!100 = !DIDerivedType(tag: DW_TAG_member, name: "box", scope: !97, file: !7, line: 24, baseType: !101, size: 128, align: 64, offset: 128)
!101 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<short>", scope: !7, file: !7, line: 17, size: 128, align: 64, elements: !102, identifier: "union.Box")
!102 = !{!103, !105}
!103 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !101, file: !7, line: 18, baseType: !104, size: 16, offset: 64)
!104 = !DIBasicType(name: "short", size: 16, encoding: DW_ATE_signed)
!105 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !101, file: !7, line: 19, baseType: !76, size: 64, offset: 64)
!106 = !DILocation(line: 49, column: 5, scope: !57)
!107 = !DILocation(line: 50, column: 5, scope: !57)
!108 = !DILocation(line: 50, column: 24, scope: !57)
!109 = !DILocation(line: 51, column: 34, scope: !57)
!110 = !DILocation(line: 52, column: 5, scope: !57)
!111 = !DILocation(line: 52, column: 27, scope: !57)
!112 = !DILocation(line: 53, column: 5, scope: !57)
!113 = !DILocation(line: 53, column: 26, scope: !57)
!114 = !DILocation(line: 54, column: 37, scope: !57)
!115 = !DILocation(line: 55, column: 1, scope: !57)
