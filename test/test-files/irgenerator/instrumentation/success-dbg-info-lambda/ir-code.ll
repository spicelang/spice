; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4, !dbg !0
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Doubled twice: %d\0A\00", align 4, !dbg !5
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Is even: %d\0A\00", align 4, !dbg !9
@printf.str.3 = private unnamed_addr constant [25 x i8] c"Counter after reset: %d\0A\00", align 4, !dbg !12
@printf.str.4 = private unnamed_addr constant [21 x i8] c"Counter via ptr: %d\0A\00", align 4, !dbg !15

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %0, i32 noundef %1) #0 !dbg !26 {
  %fct = alloca { ptr, ptr, i64 }, align 8
  %value = alloca i32, align 4
    #dbg_declare(ptr %fct, !39, !DIExpression(), !40)
  store { ptr, ptr, i64 } %0, ptr %fct, align 8, !dbg !40
    #dbg_declare(ptr %value, !41, !DIExpression(), !42)
  store i32 %1, ptr %value, align 4, !dbg !42
  %3 = load i32, ptr %value, align 4, !dbg !43
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !43
  %captures = load ptr, ptr %4, align 8, !dbg !43
  %fct1 = load ptr, ptr %fct, align 8, !dbg !43
  %5 = call i32 %fct1(i32 %3, ptr %captures), !dbg !43
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !43
  %captures2 = load ptr, ptr %6, align 8, !dbg !43
  %fct3 = load ptr, ptr %fct, align 8, !dbg !43
  %7 = call i32 %fct3(i32 %5, ptr %captures2), !dbg !43
  ret i32 %7, !dbg !44
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 !dbg !45 {
  %counter = alloca i32, align 4
  %offset = alloca i64, align 8
  %captures = alloca { ptr, i64 }, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %increment = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  %doubleFct = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr3 = alloca { ptr, ptr, i64 }, align 8
  %isEven = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr6 = alloca { ptr, ptr, i64 }, align 8
  %reset = alloca { ptr, ptr, i64 }, align 8
  %counterPtr = alloca ptr, align 8
  %fat.ptr9 = alloca { ptr, ptr, i64 }, align 8
  %printViaPtr = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %counter, !65, !DIExpression(), !66)
  store i32 10, ptr %counter, align 4, !dbg !66
    #dbg_declare(ptr %offset, !67, !DIExpression(), !68)
  store i64 5, ptr %offset, align 8, !dbg !68
  store ptr %counter, ptr %captures, align 8, !dbg !69
  %1 = load i64, ptr %offset, align 8, !dbg !69
  %2 = getelementptr inbounds nuw { ptr, i64 }, ptr %captures, i32 0, i32 1, !dbg !69
  store i64 %1, ptr %2, align 8, !dbg !69
  store ptr @_Z15lambda.L10C21.0v, ptr %fat.ptr, align 8, !dbg !69
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !69
  store ptr %captures, ptr %3, align 8, !dbg !69
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !69
  store i64 16, ptr %4, align 8, !dbg !69
    #dbg_declare(ptr %increment, !70, !DIExpression(), !69)
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !69
  store { ptr, ptr, i64 } %5, ptr %increment, align 8, !dbg !69
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %increment, i32 0, i32 1, !dbg !71
  %captures1 = load ptr, ptr %6, align 8, !dbg !71
  %fct = load ptr, ptr %increment, align 8, !dbg !71
  call void %fct(ptr %captures1), !dbg !71
  %7 = load i32, ptr %counter, align 4, !dbg !72
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7), !dbg !72
  store ptr @_Z15lambda.L18C29.0i, ptr %fat.ptr2, align 8, !dbg !73
  %9 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1, !dbg !73
  store ptr null, ptr %9, align 8, !dbg !73
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2, !dbg !73
  store i64 0, ptr %10, align 8, !dbg !73
    #dbg_declare(ptr %doubleFct, !74, !DIExpression(), !73)
  %11 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8, !dbg !73
  store { ptr, ptr, i64 } %11, ptr %doubleFct, align 8, !dbg !73
  %12 = load { ptr, ptr, i64 }, ptr %doubleFct, align 8, !dbg !75
  %13 = load i32, ptr %counter, align 4, !dbg !76
  %14 = call noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %12, i32 noundef %13), !dbg !76
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %14), !dbg !76
  store ptr @_Z15lambda.L24C27.0i, ptr %fat.ptr3, align 8, !dbg !77
  %16 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 1, !dbg !77
  store ptr null, ptr %16, align 8, !dbg !77
  %17 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 2, !dbg !77
  store i64 0, ptr %17, align 8, !dbg !77
    #dbg_declare(ptr %isEven, !78, !DIExpression(), !77)
  %18 = load { ptr, ptr, i64 }, ptr %fat.ptr3, align 8, !dbg !77
  store { ptr, ptr, i64 } %18, ptr %isEven, align 8, !dbg !77
  %19 = load i32, ptr %counter, align 4, !dbg !79
  %20 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %isEven, i32 0, i32 1, !dbg !79
  %captures4 = load ptr, ptr %20, align 8, !dbg !79
  %fct5 = load ptr, ptr %isEven, align 8, !dbg !79
  %21 = call i1 %fct5(i32 %19, ptr %captures4), !dbg !79
  %22 = zext i1 %21 to i32, !dbg !79
  %23 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %22), !dbg !79
  store ptr @_Z15lambda.L28C17.0v, ptr %fat.ptr6, align 8, !dbg !80
  %24 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr6, i32 0, i32 1, !dbg !80
  store ptr %counter, ptr %24, align 8, !dbg !80
  %25 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr6, i32 0, i32 2, !dbg !80
  store i64 0, ptr %25, align 8, !dbg !80
    #dbg_declare(ptr %reset, !81, !DIExpression(), !80)
  %26 = load { ptr, ptr, i64 }, ptr %fat.ptr6, align 8, !dbg !80
  store { ptr, ptr, i64 } %26, ptr %reset, align 8, !dbg !80
  %27 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %reset, i32 0, i32 1, !dbg !82
  %captures7 = load ptr, ptr %27, align 8, !dbg !82
  %fct8 = load ptr, ptr %reset, align 8, !dbg !82
  call void %fct8(ptr %captures7), !dbg !82
  %28 = load i32, ptr %counter, align 4, !dbg !83
  %29 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %28), !dbg !83
    #dbg_declare(ptr %counterPtr, !84, !DIExpression(), !85)
  store ptr %counter, ptr %counterPtr, align 8, !dbg !85
  %30 = load ptr, ptr %counterPtr, align 8, !dbg !86
  store ptr @_Z15lambda.L36C23.0v, ptr %fat.ptr9, align 8, !dbg !86
  %31 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr9, i32 0, i32 1, !dbg !86
  store ptr %30, ptr %31, align 8, !dbg !86
  %32 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr9, i32 0, i32 2, !dbg !86
  store i64 0, ptr %32, align 8, !dbg !86
    #dbg_declare(ptr %printViaPtr, !87, !DIExpression(), !86)
  %33 = load { ptr, ptr, i64 }, ptr %fat.ptr9, align 8, !dbg !86
  store { ptr, ptr, i64 } %33, ptr %printViaPtr, align 8, !dbg !86
  %34 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %printViaPtr, i32 0, i32 1, !dbg !88
  %captures10 = load ptr, ptr %34, align 8, !dbg !88
  %fct11 = load ptr, ptr %printViaPtr, align 8, !dbg !88
  call void %fct11(ptr %captures10), !dbg !88
  ret i32 0, !dbg !89
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L10C21.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !90 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !94
  %2 = load ptr, ptr %captures, align 8, !dbg !94
    #dbg_declare(ptr %captures, !95, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 0, DW_OP_deref), !94)
  %offset = getelementptr inbounds nuw { ptr, i64 }, ptr %2, i32 0, i32 1, !dbg !94
    #dbg_declare(ptr %captures, !96, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 8), !94)
  %3 = load ptr, ptr %2, align 8, !dbg !97
  %4 = load i32, ptr %3, align 4, !dbg !97
  %5 = add nsw i32 %4, 1, !dbg !97
  store i32 %5, ptr %3, align 4, !dbg !97
  %6 = load i64, ptr %offset, align 8, !dbg !98
  %7 = trunc i64 %6 to i32, !dbg !98
  %8 = load ptr, ptr %2, align 8, !dbg !98
  %9 = load i32, ptr %8, align 4, !dbg !98
  %10 = add nsw i32 %9, %7, !dbg !98
  store i32 %10, ptr %8, align 4, !dbg !98
  ret void, !dbg !99
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @_Z15lambda.L18C29.0i(i32 %0, ptr %1) #0 !dbg !100 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !104, !DIExpression(), !105)
  store i32 %0, ptr %x, align 4, !dbg !105
  store ptr %1, ptr %captures, align 8, !dbg !105
  %3 = load i32, ptr %x, align 4, !dbg !106
  %4 = mul nsw i32 %3, 2, !dbg !106
  ret i32 %4, !dbg !107
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i1 @_Z15lambda.L24C27.0i(i32 %0, ptr %1) #0 !dbg !108 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !113, !DIExpression(), !114)
  store i32 %0, ptr %x, align 4, !dbg !114
  store ptr %1, ptr %captures, align 8, !dbg !114
  %3 = load i32, ptr %x, align 4, !dbg !115
  %4 = srem i32 %3, 2, !dbg !115
  %5 = icmp eq i32 %4, 0, !dbg !116
  ret i1 %5, !dbg !116
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L28C17.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !117 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !121
    #dbg_declare(ptr %captures, !122, !DIExpression(DW_OP_deref), !121)
  %2 = load ptr, ptr %captures, align 8, !dbg !123
  store i32 0, ptr %2, align 4, !dbg !124
  ret void, !dbg !125
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L36C23.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !126 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !130
    #dbg_declare(ptr %captures, !131, !DIExpression(), !130)
  %2 = load ptr, ptr %captures, align 8, !dbg !132
  %3 = load i32, ptr %2, align 4, !dbg !132
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %3), !dbg !132
  ret void, !dbg !133
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!19, !20, !21, !22, !23, !24}
!llvm.ident = !{!25}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !7, line: 15, type: !18, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [host] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/test/./test-files/irgenerator/instrumentation/success-dbg-info-lambda/source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!4 = !{!0, !5, !9, !12, !15}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !7, line: 21, type: !8, isLocal: true, isDefinition: true)
!7 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!8 = !DIStringType(name: "printf.str.1", size: 152)
!9 = !DIGlobalVariableExpression(var: !10, expr: !DIExpression())
!10 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !7, line: 25, type: !11, isLocal: true, isDefinition: true)
!11 = !DIStringType(name: "printf.str.2", size: 104)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(name: "printf.str.3", linkageName: "printf.str.3", scope: !2, file: !7, line: 32, type: !14, isLocal: true, isDefinition: true)
!14 = !DIStringType(name: "printf.str.3", size: 200)
!15 = !DIGlobalVariableExpression(var: !16, expr: !DIExpression())
!16 = distinct !DIGlobalVariable(name: "printf.str.4", linkageName: "printf.str.4", scope: !2, file: !7, line: 37, type: !17, isLocal: true, isDefinition: true)
!17 = !DIStringType(name: "printf.str.4", size: 168)
!18 = !DIStringType(name: "printf.str.0", size: 104)
!19 = !{i32 8, !"PIC Level", i32 2}
!20 = !{i32 7, !"PIE Level", i32 2}
!21 = !{i32 7, !"uwtable", i32 2}
!22 = !{i32 7, !"frame-pointer", i32 0}
!23 = !{i32 7, !"Dwarf Version", i32 5}
!24 = !{i32 2, !"Debug Info Version", i32 3}
!25 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
!26 = distinct !DISubprogram(name: "applyTwice", linkageName: "_Z10applyTwicePFiiEi", scope: !7, file: !7, line: 3, type: !27, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !38)
!27 = !DISubroutineType(types: !28)
!28 = !{!29, !30, !29}
!29 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!30 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !7, file: !7, size: 192, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !31, identifier: "_lambda")
!31 = !{!32, !35, !36}
!32 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !30, file: !7, baseType: !33, size: 64, align: 64)
!33 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !34, size: 64, align: 64)
!34 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !30, file: !7, baseType: !33, size: 64, align: 64, offset: 64)
!36 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !30, file: !7, baseType: !37, size: 64, align: 64, offset: 128)
!37 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!38 = !{}
!39 = !DILocalVariable(name: "fct", arg: 1, scope: !26, file: !7, line: 3, type: !30)
!40 = !DILocation(line: 3, column: 19, scope: !26)
!41 = !DILocalVariable(name: "value", arg: 2, scope: !26, file: !7, line: 3, type: !29)
!42 = !DILocation(line: 3, column: 36, scope: !26)
!43 = !DILocation(line: 4, column: 20, scope: !26)
!44 = !DILocation(line: 5, column: 1, scope: !26)
!45 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 7, type: !46, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !48)
!46 = !DISubroutineType(types: !47)
!47 = !{!29}
!48 = !{!49, !56, !57, !58, !61}
!49 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !7, line: 10, size: 128, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !50)
!50 = !{!51, !53}
!51 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !49, file: !7, line: 10, baseType: !52, size: 64)
!52 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !29, size: 64)
!53 = !DIDerivedType(tag: DW_TAG_member, name: "offset", scope: !49, file: !7, line: 10, baseType: !54, offset: 64)
!54 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !55)
!55 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!56 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !7, line: 18, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !38)
!57 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !7, line: 24, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !38)
!58 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !7, line: 28, size: 64, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !59)
!59 = !{!60}
!60 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !58, file: !7, line: 28, baseType: !52, size: 64)
!61 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !7, line: 36, size: 64, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !62)
!62 = !{!63}
!63 = !DIDerivedType(tag: DW_TAG_member, name: "counterPtr", scope: !61, file: !7, line: 36, baseType: !64, size: 64)
!64 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 64)
!65 = !DILocalVariable(name: "counter", scope: !45, file: !7, line: 8, type: !29)
!66 = !DILocation(line: 8, column: 19, scope: !45)
!67 = !DILocalVariable(name: "offset", scope: !45, file: !7, line: 9, type: !54)
!68 = !DILocation(line: 9, column: 25, scope: !45)
!69 = !DILocation(line: 10, column: 21, scope: !45)
!70 = !DILocalVariable(name: "increment", scope: !45, file: !7, line: 10, type: !30)
!71 = !DILocation(line: 14, column: 5, scope: !45)
!72 = !DILocation(line: 15, column: 29, scope: !45)
!73 = !DILocation(line: 18, column: 29, scope: !45)
!74 = !DILocalVariable(name: "doubleFct", scope: !45, file: !7, line: 18, type: !30)
!75 = !DILocation(line: 21, column: 46, scope: !45)
!76 = !DILocation(line: 21, column: 57, scope: !45)
!77 = !DILocation(line: 24, column: 27, scope: !45)
!78 = !DILocalVariable(name: "isEven", scope: !45, file: !7, line: 24, type: !30)
!79 = !DILocation(line: 25, column: 36, scope: !45)
!80 = !DILocation(line: 28, column: 17, scope: !45)
!81 = !DILocalVariable(name: "reset", scope: !45, file: !7, line: 28, type: !30)
!82 = !DILocation(line: 31, column: 5, scope: !45)
!83 = !DILocation(line: 32, column: 41, scope: !45)
!84 = !DILocalVariable(name: "counterPtr", scope: !45, file: !7, line: 35, type: !64)
!85 = !DILocation(line: 35, column: 24, scope: !45)
!86 = !DILocation(line: 36, column: 23, scope: !45)
!87 = !DILocalVariable(name: "printViaPtr", scope: !45, file: !7, line: 36, type: !30)
!88 = !DILocation(line: 39, column: 5, scope: !45)
!89 = !DILocation(line: 40, column: 1, scope: !45)
!90 = distinct !DISubprogram(name: "lambda.L10C21", linkageName: "_Z15lambda.L10C21.0v", scope: !49, file: !7, line: 10, type: !91, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !38)
!91 = !DISubroutineType(types: !92)
!92 = !{!34, !93}
!93 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !49, size: 64)
!94 = !DILocation(line: 10, column: 21, scope: !90)
!95 = !DILocalVariable(name: "counter", scope: !90, file: !7, line: 8, type: !29)
!96 = !DILocalVariable(name: "offset", scope: !90, file: !7, line: 9, type: !54)
!97 = !DILocation(line: 11, column: 9, scope: !90)
!98 = !DILocation(line: 12, column: 9, scope: !90)
!99 = !DILocation(line: 13, column: 5, scope: !90)
!100 = distinct !DISubprogram(name: "lambda.L18C29", linkageName: "_Z15lambda.L18C29.0i", scope: !56, file: !7, line: 18, type: !101, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !38)
!101 = !DISubroutineType(types: !102)
!102 = !{!29, !103, !29}
!103 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !56, size: 64)
!104 = !DILocalVariable(name: "x", arg: 1, scope: !100, file: !7, line: 18, type: !29)
!105 = !DILocation(line: 18, column: 29, scope: !100)
!106 = !DILocation(line: 19, column: 20, scope: !100)
!107 = !DILocation(line: 20, column: 5, scope: !100)
!108 = distinct !DISubprogram(name: "lambda.L24C27", linkageName: "_Z15lambda.L24C27.0i", scope: !57, file: !7, line: 24, type: !109, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !38)
!109 = !DISubroutineType(types: !110)
!110 = !{!111, !112, !29}
!111 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!112 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !57, size: 64)
!113 = !DILocalVariable(name: "x", arg: 1, scope: !108, file: !7, line: 24, type: !29)
!114 = !DILocation(line: 24, column: 27, scope: !108)
!115 = !DILocation(line: 24, column: 42, scope: !108)
!116 = !DILocation(line: 24, column: 47, scope: !108)
!117 = distinct !DISubprogram(name: "lambda.L28C17", linkageName: "_Z15lambda.L28C17.0v", scope: !58, file: !7, line: 28, type: !118, scopeLine: 28, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !38)
!118 = !DISubroutineType(types: !119)
!119 = !{!34, !120}
!120 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 64)
!121 = !DILocation(line: 28, column: 17, scope: !117)
!122 = !DILocalVariable(name: "counter", scope: !117, file: !7, line: 8, type: !29)
!123 = !DILocation(line: 29, column: 9, scope: !117)
!124 = !DILocation(line: 29, column: 19, scope: !117)
!125 = !DILocation(line: 30, column: 5, scope: !117)
!126 = distinct !DISubprogram(name: "lambda.L36C23", linkageName: "_Z15lambda.L36C23.0v", scope: !61, file: !7, line: 36, type: !127, scopeLine: 36, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !38)
!127 = !DISubroutineType(types: !128)
!128 = !{!34, !129}
!129 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !61, size: 64)
!130 = !DILocation(line: 36, column: 23, scope: !126)
!131 = !DILocalVariable(name: "counterPtr", scope: !126, file: !7, line: 35, type: !64)
!132 = !DILocation(line: 37, column: 42, scope: !126)
!133 = !DILocation(line: 38, column: 5, scope: !126)
